#include <stdio.h>
#include <string.h>
#include "global.h"
#include "bind.h"
#include "display.h"
#include "heartbeat.h"
#include <net/gnrc/ipv6/netif.h>

void send_bind(void){
	bind_t pkg;
	pkg.type = BIND;
	pkg.mi = get_ipv6_addr();
	udp_send(&pkg, sizeof(pkg), NULL);	// Send multicast
	heartbeat_timeout_init(); // Start timer when bind was send
	free(pkg.mi);
}

void handle_bind(bind_t* p) {
	//Start heartbeat_send 
	printDisplayHopAndFoundActive(); // Display Hop and Found active!
	heartbeat_sender_init(p->mi); // Start to send hearbeat

}