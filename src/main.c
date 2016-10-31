#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"

#define NETIF_CHANNEL ((int16_t) 11)	//The channel the udp transmitter and receiver will use.
#define NETIF_POWER ((int16_t) -21)		//Tx value in dB

int main(void){
	int sn;
	packet_t p;
	int error_ret;
	
	xtimer_sleep(1);
	
	error_ret = set_netif(CHANNEL, NETIF_CHANNEL);
	if (error_ret != 0){
		printf("Failed to set netif channel to %d, error_ret=%d\n", NETIF_CHANNEL, error_ret);
	} else {
		printf("Set netif channel to %d\n", NETIF_CHANNEL);
	}
	
	error_ret = set_netif(POWER, NETIF_POWER);
	if (error_ret != 0){
		printf("Failed to set netif power to %d, error_ret=%d\n", NETIF_POWER, error_ret);
	} else {
		printf("Set netif power to %d\n", NETIF_POWER);
	}
	
	if (udp_server_start() != 0){
		printf("Failed to start udp server\n");
	} else {
		printf("Start UDP server\n");
	}
	
	p.header = 1;
	p.message = 42;
	
	sn = 0;
	while(1)
	{
		p.sequence_number = sn++;
		udp_send(&p, sizeof(p), NULL);
		print_packet("Sent Packet", &p);
		xtimer_sleep(1);
	}

	return 0;
}
