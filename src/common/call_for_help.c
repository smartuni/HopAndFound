#include <stdio.h>
#include <string.h>
#include "global.h"
#include "call_for_help.h"
#include "display.h"
#include "console_map.h"
#include "localization_reply.h"
#include "xtimer.h"


#define CALL_FOR_HELP_TIME_USEC		(15000000)

static int seq_nr_send = 1;
static int seq_nr_recv = 0;


xtimer_t _timer_call_for_help;


void _call_for_help_handler_task(void) {
	printDisplayHopAndFoundActive();
}

void call_for_help_handler_init(void) {
    _timer_call_for_help.target = 0;
    _timer_call_for_help.long_target = 0;
    _timer_call_for_help.callback = (void*)_call_for_help_handler_task;
	xtimer_set(&_timer_call_for_help, CALL_FOR_HELP_TIME_USEC);
}

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
	memcpy(&pkg.node_list, get_node_list(), MAX_NODES);
	udp_send(&pkg, sizeof(pkg), NULL);	
	
/*#ifdef HAF_DEBUG_NODE_MAP
	printConsoleMap(get_node_list(), MAX_NODES);
#endif*/

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

#ifdef TEST_PRESENTATION
		p->node_list_path[NODE_ID] = 1;
#endif /* TEST_PRESENTATION */
		udp_send(&pkg, sizeof(pkg), NULL);
		seq_nr_recv = p->seq_nr;
	}
}

void handle_call_for_help(call_for_help_t* p, handler_t h) {
	if ( h == NODE ) {
		forward_call_for_help(p);
	} else if ( h == MONITOR ) {
		if ( p->mi_id == MONITORED_ITEM_ID ) {
			//printDisplayMapString(p->node_list);
			printDisplayMapStringPath(p->node_list,p->node_list_path);
		    xtimer_set(&_timer_call_for_help, CALL_FOR_HELP_TIME_USEC);
#ifdef HAF_DEBUG_DONT_PRINT_EMPTY_MAP
			int i;
			
			for(i = 0; i < MAX_NODES; i++){
				if (p->node_list[i] == 1){
					printConsoleMap(p->node_list, MAX_NODES);
					break;
				}
			}
#else
			printConsoleMap(p->node_list, MAX_NODES);
#endif
		}
	}
}
