#include <stdio.h>
#include <string.h>
#include "global.h"
#include "call_for_help.h"
#include "console_map.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "display.h"

#define CALL_FOR_HELP_TIME_USEC		(2000000)

static int seq_nr_send = 1;
static int seq_nr_recv = 0;

xtimer_t _timer_call_for_help;

void _call_for_help_handler_task(void) {
	printDisplayHopAndFound();
}

void call_for_help_handler_init(void) {
    _timer_call_for_help.target = 0;
    _timer_call_for_help.long_target = 0;
    _timer_call_for_help.callback = (void*)_call_for_help_handler_task;
	xtimer_set(&_timer_call_for_help, CALL_FOR_HELP_TIME_USEC);
}

void send_call_for_help(void) {
	call_for_help_t pkg;

	pkg.type = CALL_FOR_HELP;

	if ( seq_nr_send >= 1000000 ) { //Abfrage im Empfang berücksichtigen
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


void forward_call_for_help(call_for_help_t* p) {
	if (p->seq_nr > seq_nr_recv){
#ifdef HAF_DEBUG
			printf("CALL FOR HELP forward.\n");
#endif
#ifdef TEST_PRESENTATION
		p->node_list_path[NODE_ID] = 1;
#endif /* TEST_PRESENTATION */
		udp_send(p, sizeof(p), NULL);
		seq_nr_recv = p->seq_nr;
	}
}

void handle_call_for_help(call_for_help_t* p, handler_t h) {
	if ( h == NODE ) {
		forward_call_for_help(p);
	} else if ( h == MONITOR ) {
		if ( p->mi_id == MONITORED_ITEM_ID ) {
			printDisplayMapString(p->node_list);
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
