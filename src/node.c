#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "connection.h"

#define UDP_RECV_PORT ((uint16_t) 9999) 	//Outer port for udp communication
#define UDP_SRC_PORT ((uint16_t) 1234)		//Inner port fpr udp communication
#define UDP_MULTICAST_ADDRESS ("ff02::1")	//IPv6 multicast address

#define MAX_RECV_BUFFER_SIZE 12
#define NODE_ID 1

void dispatcher(void){
	
    uint16_t port = UDP_RECV_PORT;
    conn_udp_t conn;
	char[MAX_RECV_BUFFER_SIZE] recv_buffer;
	
    ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
	
    msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if(conn_udp_create(&conn, &server_addr, sizeof(server_addr), AF_INET6, port) < 0) {
		printf("Cannot create connection on port %d\n", port);
		// TODO error handling
        return NULL;
    }

    while (1) {
        int res;
        ipv6_addr_t src;
        size_t src_len = sizeof(ipv6_addr_t);
        if ((res = conn_udp_recvfrom(&conn, &recv_buffer, sizeof(recv_buffer),
                                     &src, &src_len, &port)) < 0) {
            puts("Error while receiving\n");            
            // TODO error handling
        }
        else if (res == 0) {
            puts("No data received\n");
            // TODO error handling
        }
        else {
            print_packet("Received Packet", &recv_buffer);
			
			switch(recv_buffer[0]){
				case LOCALIZATION_REQUEST: 
					printf(" %d\n", port);
					break;
					
				case CALL_FOR_HELP:				
							
					break;
			}
        }
    }

    return NULL;
}



int main(void){
	disptacher();
	return 0;
}
