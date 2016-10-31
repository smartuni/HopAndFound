static int seq_nr_send = 1;
static int seq_nr_recv = 0;

void send_call_for_help(void) {
	call_for_help_t pkg;

	pkg.type = CALL_FOR_HELP;

	if ( seq_nr_send < 1000000 ) { //Abfrage im Empfang berÃcksichtigen
		pkg.seq_nr = seq_nr_send;
		seq_nr_send++;
	} else {
		seq_nr_send = 0;
		pkg.seq_nr = seq_nr_send;
		seq_nr_send++;
	}
	pkg.mi_id = MONITORED_ITEM_ID; //<-------------DEFINE ANLEGEN
	pkg.node_list = get_node_list(); //<-----------getter fÃr nodelist
	udp_send(&pkg, sizeof(pkg), NULL);	
}


void forward_call_for_help(call_for_help_t* p) {
	udp_send(&p, sizeof(p), NULL);
}

void handle_call_for_help(call_for_help_t* p, handler_t h) {
	int i;
	if ( handler_t == NODE ) {
		forward_call_for_help(&p);
	} else if ( handler_t == MONITOR ) {
		if ( pkg.mi_id == MONITORED_ITEM_ID ) {
			if (pkg.seq_nr > seq_nr_recv ) {
				seq_nr_recv = pkg.seq_nr;
			}
		}
	}
}
