#include <stdio.h>
#include <string.h>
#include "global.h"
#include "call_for_help.h"
#include "display.h"
#include "console_map.h"
#include "localization_reply.h"
#include "xtimer.h"
#include "routing.h"

#define CALL_FOR_HELP_TIME_USEC		(15000000)

static int seq_nr_send = 1;
static int seq_nr_recv = 0;


xtimer_t _timer_call_for_help;

/*  function _call_for_help_handler_task
 *
 *	Calls the printDisplayHopAndFound function
 *
 */
void _call_for_help_handler_task(void) {
	printDisplayHopAndFoundActive();
	puts("call_for_help_timeout");
}

/*  function call_for_help_handler_init
 *
 *	Initializes the timer for sending the Call For Help message
 *
 */
void call_for_help_handler_init(void) {
	puts("call_for_help_handler_init");
    _timer_call_for_help.target = 0;
    _timer_call_for_help.long_target = 0;
    _timer_call_for_help.callback = (void*)_call_for_help_handler_task;
}

/*  function send_call_for_help
 *
 *	Sends the Call For Help from the monitor through the node
 *
 */
void send_call_for_help(void) {
	call_for_help_t pkg;
	ipv6_addr_t routed_dst;
	memcpy(&pkg.dest_adr, getMonitorIP(), sizeof(ipv6_addr_t));
	pkg.type = CALL_FOR_HELP;

	if ( seq_nr_send >= 1000000 ) { //Abfrage im Empfang beruecksichtigen
		seq_nr_send = 0;
	}

	pkg.seq_nr = seq_nr_send;
	seq_nr_send++;

	pkg.mi_id = MONITORED_ITEM_ID;
	memcpy(&pkg.node_list, get_node_list(), MAX_NODES);
	get_hops_p(&pkg.ttl);
	pkg.ttl++;
	get_route_p(&routed_dst);
	udp_send(&pkg, sizeof(pkg), &routed_dst);
	printf("CALL FOR HELP SENT TO "); print_ipv6_string(&routed_dst); printf("\n");
	printf("CALL FOR HELP DEST IP: "); print_ipv6_string(&pkg.dest_adr); printf("\n");
	clear_route_list();
	resetNodeList();
}

/*  function forward_call_for_help
 *
 *	Forwards the Call For Help through the next node or the monitor [the hop address]
 *
 *	@param p Call For Help message
 */
void forward_call_for_help(call_for_help_t* p) {
	call_for_help_t pkg;
	if (p->seq_nr > seq_nr_recv){
#ifdef HAF_DEBUG
		printf("CALL FOR HELP forward.\n");
		printf("type: %u\n", p->type);
		printf("seq_nr: %lu\n", p->seq_nr);
		printf("mi_id: %u\n", p->mi_id);
		printf("ttl: %u\n", p->ttl);
		printf("dest_adr: "); print_ipv6_string(&p->dest_adr); printf("\n");
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
		sendpkg(&pkg);
		seq_nr_recv = p->seq_nr;
	}
}
/*  function forward_call_for_help
 *
 *	Forwards the Call For Help through the next node or the monitor [the hop address]
 *
 *	@param p Call For Help message
 *	@h device type
 */
void handle_call_for_help(call_for_help_t* p, handler_t h) {
	if ( h == NODE ) {
		forward_call_for_help(p);
	} else if ( h == MONITOR ) {
		if ( p->mi_id == MONITORED_ITEM_ID ) {
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
