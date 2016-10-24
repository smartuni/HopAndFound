#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "net/af.h"
#include "net/conn/udp.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "connection.h"
#include "xtimer.h"



#define SERVER_MSG_QUEUE_SIZE   (8)
#define SERVER_BUFFER_SIZE      (sizeof(packet_t))



static bool server_running = false;
static conn_udp_t conn;
static char server_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
static kernel_pid_t netif_dev = -1;



void print_packet(char* info_text, packet_t* p){
	printf("%s (t=%u):: header=%d, sequence_number=%d, message=%d\n", 
				info_text, (uint) xtimer_now(), p->header, p->sequence_number, p->message);
}

void* _udp_server(void *args){
    uint16_t port = UDP_RECV_PORT;
    packet_t server_buffer;
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
        if ((res = conn_udp_recvfrom(&conn, &server_buffer, sizeof(server_buffer),
                                     &src, &src_len, &port)) < 0) {
            puts("Error while receiving");            
            // TODO error handling
        }
        else if (res == 0) {
            puts("No data received");
            // TODO error handling
        }
        else {
            print_packet("Received Packet", &server_buffer);
        }
    }

    return NULL;
}

int udp_send(packet_t* p){
	int res;
    ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED, dst;
    if (ipv6_addr_from_str(&dst, UDP_MULTICAST_ADDRESS) == NULL) {
        return -1;
    }
    
    res = conn_udp_sendto(p, sizeof(*p), &src, sizeof(src), &dst, sizeof(dst), AF_INET6, UDP_SRC_PORT, UDP_RECV_PORT);
    
    return res;
}

int udp_server_start(void){
    if ((server_running == false) &&
        thread_create(server_stack, sizeof(server_stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, _udp_server, NULL, "HopAndFound UDP Server")
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
		printf("IFID=%d\n", ifs[0]);
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
