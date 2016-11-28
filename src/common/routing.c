#include <stdio.h>
#include <string.h>

#include "xtimer.h"

#include "global.h"
#include "routing.h"
#include "connection.h"
#include "HAF_protocol.h"

#define TIMEOUT 9000000 //10sek.
#define EXP_TIMEOUT 30000000 //60sek.

xtimer_t timer_update;

routing_tbl_t routing_tbl[MAX_DEVICES];


void init(void) { //muss in der main aufgerufen werden
	xtimer_init();
    timer_update.target = 0;
    timer_update.long_target = 0;
	timer_update.callback = (void*)_update;
	xtimer_set(&timer_update, TIMEOUT);
	routing_tbl[0].mac_adr = DEV_MAC_ADR; //DEVICE MAC ADRESS
	routing_tbl[0].hops = 0;
	routing_tbl[0].next_hop_adr = 0;
	routing_tbl[0].exp_time = 0;
	for (int i=1;i<MAX_DEVICES;i++) {
		routing_tbl[i].mac_adr = 0;
		routing_tbl[i].hops = 0;
		routing_tbl[i].next_hop_adr = 0;
		routing_tbl[i].exp_time = 0;
	}
	//routing_tbl[1].exp_time = ( xtimer_now() + EXP_TIMEOUT );
	//routing_tbl[2].exp_time = ( xtimer_now() + EXP_TIMEOUT );
	//routing_tbl[3].exp_time = ( xtimer_now() + EXP_TIMEOUT );
	
	puts("Init complete - start sending");
	_update();
}

void _update(void){
	printf("update funktion gestartet\n");
	//printf("systemzeit: %" PRIu32 "\n", xtimer_now() );
	//printf("exp time: %" PRIu32 "\n", ( xtimer_now() + EXP_TIMEOUT ) );
	update_t pkg;
	xtimer_set(&timer_update, TIMEOUT);
	check_exp();
	pkg.type = UPDATE;
	pkg.source_adr = DEV_MAC_ADR;
	for (int i=0;i<MAX_DEVICES;i++) {
		pkg.routing_tbl[i].mac_adr = routing_tbl[i].mac_adr;
		pkg.routing_tbl[i].hops = routing_tbl[i].hops;
		pkg.routing_tbl[i].next_hop_adr = routing_tbl[i].next_hop_adr;
		pkg.routing_tbl[i].exp_time = routing_tbl[i].exp_time;
	}
	printf("local package presend:\n");
	for(int i = 0; i < MAX_DEVICES; i++) {
		printf("routing_tbl[%d].mac_adr: %u\n", i, pkg.routing_tbl[i].mac_adr);
		printf("routing_tbl[%d].hops: %u\n", i, pkg.routing_tbl[i].hops);
		printf("routing_tbl[%d].next_hop_adr: %u\n", i, pkg.routing_tbl[i].next_hop_adr);
		printf("routing_tbl[%d].exp_time: %" PRIu32 "\n", i, pkg.routing_tbl[i].exp_time);
	}
	udp_send(&pkg, sizeof(pkg), NULL);
}

void handle_update(update_t* p, uint32_t source_adr){
	static int found, change, remove = 0;
	printf("empfagene routing tbl:\n");
	for(int i = 0; i < MAX_DEVICES; i++) {
		printf("p[%d].mac_adr: %u\n", i, p->routing_tbl[i].mac_adr);
		printf("p[%d].hops: %u\n", i, p->routing_tbl[i].hops);
		printf("p[%d].next_hop_adr: %u\n", i, p->routing_tbl[i].next_hop_adr);
		printf("p[%d].exp_time: %" PRIu32 "\n", i, p->routing_tbl[i].exp_time);
	}
	for (int i=0;i<MAX_DEVICES;i++) { //empfangene routing table durcharbeiten
		found = 0;
		if ( p->routing_tbl[i].mac_adr != 0 ) {
		//printf("bearbeite empfangene routing table - eintrag: %d:\n", i);
		for (int j=0;j<MAX_DEVICES;j++) { //eintrag in lokaler routing table suchen
			//printf("bearbeite in lokaler routing table - eintrag %d, zum vergleich mit empf. rt eintr: %d:\n", j, i);
			if ( ( p->routing_tbl[i].mac_adr == routing_tbl[j].mac_adr ) && found == 0 )  { //eintrag in lokaler routing table gefunden
				found = 1;
				//printf("eintrag %d von empf. rt in lok. rt eintr. %d gefunden\n", i, j);
				if ( p->routing_tbl[i].hops == 0 ) { //eintrag direkter nachbar?
					printf("empf. rt element %d ist ein direkter nachbar, expiration time wird erneuert in lok. rt element %d\n", i, j);
					routing_tbl[j].hops = 1;
					routing_tbl[j].next_hop_adr = routing_tbl[j].mac_adr;
					routing_tbl[j].exp_time = xtimer_now() + EXP_TIMEOUT;
				} else { //eintrag kein direkter nachbar
					//printf("empf. rt element %d ist kein direkter nachbar\n", i);
					if ( p->routing_tbl[i].hops < routing_tbl[j].hops ) { //prüfe ob route kürzer ist
						printf("empf. rt element %d ist KEIN direkter nachbar, expiration time wird erneuert in lok. rt element %d\n", i, j);
						if ( routing_tbl[j].next_hop_adr !=  source_adr ) { //prüfe ob route über anderen nachbarn geroutet wird
						//if ( p->routing_tbl[i].next_hop_adr !=  source_adr ) { //prüfe ob route über anderen nachbarn geroutet wird
							printf("CHANGE empf. rt element %d route wird geändert in lok. rt element %d\n", i, j);
							change = 1;
							routing_tbl[j].next_hop_adr = source_adr;
						}
						routing_tbl[j].hops = p->routing_tbl[i].hops+1;
						routing_tbl[j].exp_time = xtimer_now() + EXP_TIMEOUT;
					}
				}
			}
		}
		if ( found == 0 ) { //eintrag in lokaler routing table nicht gefunden
			//printf("es wurde kein eintrag in lokaler rt zum eintrag %d der empf. rt gefunden\n", i);
			for (int j=0;j<MAX_DEVICES;j++) { //suche nach freiem eintrag in lokaler routing table
				//printf("pruefe, ob eintrag %d in lok. rt frei ist\n", j);
				if ( routing_tbl[j].mac_adr == 0 && found == 0) { //freier eintrag in lokaler routing table gefunden
					printf("CHANGE empf. rt element %d eintragen in lok. rt element %d\n", i, j);
					found = 1;
					change = 1;
					routing_tbl[j].mac_adr = p->routing_tbl[i].mac_adr; //eintrag in lokale routing table eintragen
					routing_tbl[j].hops = p->routing_tbl[i].hops + 1;
					routing_tbl[j].next_hop_adr = source_adr;
					routing_tbl[j].exp_time = xtimer_now() + EXP_TIMEOUT;
				}
			}
		}
	}
	}
	for (int j=0;j<MAX_DEVICES;j++) {
		if ( source_adr == routing_tbl[j].next_hop_adr ) { //prüfung auf weggefallene route beim sender
			remove = 1;
			//printf("pruefe ob die lok. route %d in der empf. rt noch vorhanden\n",j);
			for (int k=0;k<MAX_DEVICES;k++) { //empfangene routing table durcharbeiten
			//printf("eintrag %d in empf. rt durcharbeiten\n", k);
				if ( routing_tbl[j].mac_adr == p->routing_tbl[k].mac_adr && remove == 1 ) { //passende route gefunden
				//printf("empf. rt element %d route gefunden, nichts unternehmen in lok. rt element %d\n", k, j);
					remove = 0; //es muss nichts gelöscht werden
					//found = 1;
				}
			}
			if ( remove == 1 ) { //passende route nicht gefunden, daher route löschen
				printf("CHANGE in empf. rt element nicht gefunden, loeschen in lok. rt element %d\n", j);
				routing_tbl[j].mac_adr = 0;
				routing_tbl[j].hops = 0;
				routing_tbl[j].next_hop_adr = 0;
				routing_tbl[j].exp_time = 0;
				change = 1;
				//found = 1;
			}
		}
	}
	//printf("empfangene routing table fertig durchgearbeitet\n");
	if ( change == 1 ) {
		change = 0;
		printf("es wurden aenderungen vorgenommen, routing table wird neu gesendet\n");
		_update();
	}
}

void check_exp(void){
	//exp time der routing table einträge prüfen
	for (int j=1;j<MAX_DEVICES;j++) { //lokale routing table durcharbeiten
		if ( routing_tbl[j].exp_time <= xtimer_now() ) { //prüfe ob eintrag veraltet
			routing_tbl[j].mac_adr = 0;
			routing_tbl[j].hops = 0;
			routing_tbl[j].next_hop_adr = 0;
			routing_tbl[j].exp_time = 0;
		}
	}
}

bool checkroute(call_for_help_t* p) {
	p->ttl = p->ttl - 1;
	for (int j=1;j<MAX_DEVICES;j++) {
		if (p->dest_adr == routing_tbl[j].mac_adr ) {
			//if (routing_tbl[j].hops <= p->ttl ) {
				return true;
			//}
		}
	}
	return false;
}

