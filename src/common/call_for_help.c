#include <stdio.h>
#include <string.h>
#include "global.h"
#include "call_for_help.h"
#include "console_map.h"
#include "localization_reply.h"

static int seq_nr_send = 1;
static int seq_nr_recv = 0;

void send_call_for_help(void) {
	call_for_help_t pkg;
	pkg.dest_adr = MONITOR_ID;
	pkg.ttl = 99;
	pkg.type = CALL_FOR_HELP;

	if ( seq_nr_send >= 1000000 ) { //Abfrage im Empfang beruecksichtigen
		seq_nr_send = 0;
	}
	
	pkg.seq_nr = seq_nr_send;
	seq_nr_send++;
		
	pkg.mi_id = MONITORED_ITEM_ID;
	memcpy(&pkg.node_list, get_node_list(), MAX_NODES); //<-----------getter fuer nodelist
	udp_send(&pkg, sizeof(pkg), NULL);	
	
#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif

resetNodeList();
}

// Fuer den ersten Milestone wird die mi_id nicht mit ausgewertet
//Seq_nr_recv abfrage fuer limit erreicht (1000000) fehlt
void forward_call_for_help(call_for_help_t* p) {
	call_for_help_t pkg;
	if (p->seq_nr > seq_nr_recv){
		#ifdef HAF_DEBUG
			printf("CALL FOR HELP forward.\n");
			printf("type: %u\n", p->type);
			printf("seq_nr: %lu\n", p->seq_nr);
			printf("mi_id: %u\n", p->mi_id);
			printf("ttl: %u\n", p->ttl);
			printf("dest_adr: %u\n", p->dest_adr);
		#endif
		pkg.type = p->type;
		pkg.seq_nr = p->seq_nr;
		pkg.mi_id = p->mi_id;
		for(int i = 0; i < MAX_NODES; i++) {
			pkg.node_list[i] = p->node_list[i];
		}
		pkg.ttl = p->ttl;
		pkg.dest_adr = p->dest_adr;
		udp_send(&pkg, sizeof(pkg), NULL);
		//udp_send(p, sizeof(p), NULL);
		seq_nr_recv = p->seq_nr;
	}
}

void handle_call_for_help(call_for_help_t* p, handler_t h) {
	if ( h == NODE ) {
		forward_call_for_help(p);
	} else if ( h == MONITOR ) {
		if ( p->mi_id == MONITORED_ITEM_ID ) { //Fuer den ersten Milestone kann ein Monitor nur ein Monitored Item ueberwachen
			printConsoleMap(p->node_list, MAX_NODES); //Fuer den ersten Milestone wird die node_list noch nicht an die console_map uebergeben
		}
	}
}
