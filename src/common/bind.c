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
	ipv6_addr_t* ipMI;
	ipMI = malloc(sizeof(ipv6_addr_t));
	get_ipv6_addr(ipMI);
	pkg.mi = ipMI;
	
	char ipv6_MI[IPV6_ADDR_MAX_STR_LEN];
	ipv6_addr_to_str(ipv6_MI, ipMI, IPV6_ADDR_MAX_STR_LEN);
    printf("SEND UDP PKG TO %s\n", ipv6_MI);
	
	udp_send(&pkg, sizeof(pkg), NULL);	// Send multicast
	heartbeat_timeout_init(); // Start timer when bind was send
	free(ipMI);
}

void handle_bind(bind_t* p) {
	//Start heartbeat_send 
	printDisplayHopAndFoundActive(); // Display Hop and Found active!
	heartbeat_sender_init(p->mi); // Start to send hearbeat

}