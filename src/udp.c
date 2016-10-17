#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "udp.h"

#define SERVER_MSG_QUEUE_SIZE   (8)
#define SERVER_BUFFER_SIZE      (sizeof(TPacket))

static bool server_running = false;
static conn_udp_t conn;
static TPacket server_buffer[SERVER_BUFFER_SIZE];
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

void *_udp_server(void *args)
{
    uint16_t port = (uint16_t) atoi(args);
    ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if(conn_udp_create(&conn, &server_addr, sizeof(server_addr), AF_INET6, port) < 0) {
		// TODO error handling
        return NULL;
    }

    server_running = true;

    while (1) {
        int res;
        ipv6_addr_t src;
        size_t src_len = sizeof(ipv6_addr_t);
        if ((res = conn_udp_recvfrom(&conn, server_buffer, sizeof(server_buffer),
                                     &src, &src_len, &port)) < 0) {
            puts("Error while receiving");            
            // TODO error handling
        }
        else if (res == 0) {
            puts("No data received");
            // TODO error handling
        }
        else {
            switch(server_buffer.header){
				// TODO function calling
			}
        }
    }

    return NULL;
}

int udp_send(TPacket* p)
{
    int res;
    ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED, dst;
    if (ipv6_addr_from_str(&dst, UDP_MULTICAST_ADDRESS) == NULL) {
        return 1;
    }
    
    return conn_udp_sendto(payload, strlen(payload), &src, sizeof(src), &dst, sizeof(dst), AF_INET6, UDP_SRC_PORT, UDP_PORT);
}

int udp_server(void)
{
    if ((server_running == false) &&
        thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, _udp_server, UDP_PORT, "HopAndFound UDP Server")
        <= KERNEL_PID_UNDEF) {
        return -1;
    }

    return 0;
}
