#include <stdio.h>
#include <string.h>
#include "global.h"
#include "bind.h"
#include "display.h"
#include "heartbeat.h"
#include "call_for_help.h"
#include <net/gnrc/ipv6/netif.h>

void send_bind(void){
	bind_t pkg;
	pkg.type = BIND;
	
	udp_send(&pkg, sizeof(pkg), NULL);	// Send multicast
}

void send_bind_ack(ipv6_addr_t* miIP){
	bind_t pkg;
	pkg.type = BIND_ACK;
	
	udp_send(&pkg, sizeof(pkg), miIP);	// Send multicast
	puts("Send Ack");
}


void handle_bind(ipv6_addr_t* miIP) {
	//Start heartbeat_send 
	printDisplayHopAndFoundActive(); // Display Hop and Found active!
	call_for_help_handler_init();
	heartbeat_sender_init(miIP); // Start to send hearbeat
	send_bind_ack(miIP);
}
