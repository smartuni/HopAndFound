#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"

#include "udp_server.h"

#define SERVER_MSG_QUEUE_SIZE   (8)
#define MAX_RECV_BUFFER_SIZE	(12)

void udp_server(uint16_t port, dispatcher_callback_t cb) {
    conn_udp_t conn;
	uint8_t recv_buffer[MAX_RECV_BUFFER_SIZE];
    ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
    msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if(conn_udp_create(&conn, &server_addr, sizeof(server_addr), AF_INET6, port) < 0) {
		printf("Cannot create connection on port %d\n", port);
		// TODO error handling
    }

    while (1) {
        int res;
        ipv6_addr_t src;
        size_t src_len = sizeof(ipv6_addr_t);
		memset(recv_buffer, 0, MAX_RECV_BUFFER_SIZE);
        if((res = conn_udp_recvfrom(&conn, &recv_buffer, sizeof(recv_buffer),
									&src, &src_len, &port)) < 0) {
			puts("Error while receiving\n");            
			// TODO error handling
        } else if(res == 0) {
            puts("No data received\n");
            // TODO error handling
        } else {
            cb(recv_buffer);
        }
    }
}
