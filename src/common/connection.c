#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "thread.h"
#include "connection.h"
#include "HAF_protocol.h"
#include "xtimer.h"
#include "global.h"



#define SERVER_MSG_QUEUE_SIZE   (128)
#define MAX_RECV_BUFFER_SIZE	(sizeof(call_for_help_t))


static char stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t netif_dev = -1;


void* _udp_server(void *args) {
	uint16_t port = UDP_RECV_PORT;
	dispatcher_callback_t cb = (dispatcher_callback_t) args;
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
		memset(recv_buffer, 0, sizeof(recv_buffer));
        if((res = conn_udp_recvfrom(&conn, &recv_buffer, sizeof(recv_buffer),
									&src, &src_len, &port)) < 0) {
			puts("Error while receiving\n");            
			// TODO error handling
        } else if(res == 0) {
            puts("No data received\n");
            // TODO error handling
        } else {
            cb(recv_buffer, &src);
        }
    }
}



int udp_send(void* p, size_t p_size, ipv6_addr_t* dst){
	int res;
    ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED;
    ipv6_addr_t d = IPV6_ADDR_UNSPECIFIED;
    
    if (dst != NULL){
		d = *dst;
	}
    
    if (ipv6_addr_from_str(&d, UDP_MULTICAST_ADDRESS) == NULL) {
        return -1;
    }
    
    res = conn_udp_sendto(p, p_size, &src, sizeof(src), &d, sizeof(*dst), AF_INET6, UDP_SRC_PORT, UDP_RECV_PORT);
    
    return res;
}

int udp_server_start(dispatcher_callback_t cb){
    if (thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, _udp_server, cb, "HopAndFound UDP Server")
        <= KERNEL_PID_UNDEF) {
        return -1;
    }

    return 0;
}

kernel_pid_t _get_netif(void){
	if (netif_dev != -1){
		return netif_dev;
	}
	
	kernel_pid_t ifs[GNRC_NETIF_NUMOF];
	size_t numof = gnrc_netif_get(ifs);
	
	if (numof == 1){
#ifdef HAF_DEBUG
		printf("IFID=%d\n", ifs[0]);
#endif
		netif_dev = ifs[0];
		return ifs[0];
	} else {
		return -1;
	}
}


int set_netif(netif_mode_t mode, int16_t val){
	int res;
	kernel_pid_t dev = _get_netif();
	
	if (dev == -1){
		return 3;
	}
	
	switch (mode){
		case POWER:
			res = gnrc_netapi_set(dev, NETOPT_TX_POWER, 0, (int16_t *)&val, sizeof(int16_t));
			break;
		case CHANNEL:
			if (val < 0){
				return 2;
			}
			res = gnrc_netapi_set(dev, NETOPT_CHANNEL, 0, (int16_t *)&val, sizeof(int16_t));
			break;
		default:
			return 1;	
	}
	
	return res < 0 ? res : 0;
}
