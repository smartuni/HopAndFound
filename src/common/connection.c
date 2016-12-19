#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "connection.h"
#include "HAF_protocol.h"
#include "xtimer.h"
#include "global.h"
#include <net/gnrc/ipv6/netif.h>

#ifdef HAF_USE_SOCK_UDP
#include "net/sock/udp.h"
#else
#include "net/conn/udp.h"
#endif


#define SERVER_MSG_QUEUE_SIZE   (8)
#define MAX_RECV_BUFFER_SIZE	(sizeof(update_t))

#define SEND_SLEEP_TIMER		(100000)
#define RECV_DROP_DIFF_TIME		(SEND_SLEEP_TIMER/2)


static kernel_pid_t netif_dev = -1;

static ipv6_addr_t monitor_ip = IPV6_ADDR_UNSPECIFIED;

#ifdef HAF_USE_SOCK_UDP
int udp_server_start(dispatcher_callback_t cb) {
	sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
	sock_udp_t sock;
	uint8_t recv_buffer[MAX_RECV_BUFFER_SIZE];

	local.port = UDP_RECV_PORT;

	if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
		puts("[SOCK] Error creating UDP sock");
		return 1;
	}

	uint32_t last_src_rcv_time = 0;
	char src_str[IPV6_ADDR_MAX_STR_LEN];
	char last_src[IPV6_ADDR_MAX_STR_LEN];
	while (1) {
		sock_udp_ep_t remote;
		ssize_t res;
		if ((res = sock_udp_recv(&sock, recv_buffer, sizeof(recv_buffer),
									SOCK_NO_TIMEOUT, &remote)) < 0) {
			printf("[SOCK] Error while receiving %d\n", res);
		} else if (res == 0) {
			puts("[SOCK] No data received");
		} else {
			ipv6_addr_to_str(src_str, (ipv6_addr_t*) &remote.addr, IPV6_ADDR_MAX_STR_LEN);

			if (strcmp(last_src, src_str) == 0 && xtimer_now().ticks32 - last_src_rcv_time < RECV_DROP_DIFF_TIME){
#ifdef HAF_DEBUG
				printf("[SOCK] PACKET DROPPED FROM UDP SERVER\n");
#endif
				continue;
			}
#ifdef HAF_DEBUG
			printf("[SOCK] RECEIVED UDP PKG FROM %s\n", src_str);
#endif
			last_src_rcv_time = xtimer_now().ticks32;
			memcpy(&last_src, &src_str, sizeof(src_str));
            cb(recv_buffer, (ipv6_addr_t*) &remote.addr.ipv6);
    	}
    }
}

int udp_send(void* p, size_t p_size, ipv6_addr_t* dst){
	int res;
    ipv6_addr_t d = IPV6_ADDR_UNSPECIFIED;
#ifdef HAF_DEBUG
	char src_str[IPV6_ADDR_MAX_STR_LEN];
#endif

    if (dst != NULL){
		//memcpy(&d, dst, sizeof(*dst));
		d = *dst;
	}else{
		if (ipv6_addr_from_str(&d, UDP_MULTICAST_ADDRESS) == NULL) {
			return -1;
		}
	}

    xtimer_usleep(NODE_ID * SEND_SLEEP_TIMER);

#ifdef HAF_DEBUG
    ipv6_addr_to_str(src_str, &d, IPV6_ADDR_MAX_STR_LEN);
    printf("[SOCK] SEND UDP PKG TO %s\n", src_str);
#endif

    sock_udp_ep_t remote = { 	.family = AF_INET6,
								.port = UDP_RECV_PORT};
	memcpy(&remote.addr.ipv6, &d, sizeof(ipv6_addr_t));

    res = sock_udp_send(NULL, p, p_size, &remote);

#ifdef HAF_DEBUG
    if (res < 0)
		printf("[SOCK] UDP SEND ERROR %d\n", res);
#endif

    return res;
}

#else

int udp_server_start(dispatcher_callback_t cb) {
	uint16_t port = UDP_RECV_PORT;
    conn_udp_t conn;
	uint8_t recv_buffer[MAX_RECV_BUFFER_SIZE];
	ipv6_addr_t server_addr = IPV6_ADDR_UNSPECIFIED;
	msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];
	msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

	if(conn_udp_create(&conn, &server_addr, sizeof(server_addr), AF_INET6, port) < 0) {
		printf("[CONN] Cannot create connection on port %d\n", port);
		// TODO error handling
	}

	uint32_t last_src_rcv_time = 0;
	char src_str[IPV6_ADDR_MAX_STR_LEN];
	char last_src[IPV6_ADDR_MAX_STR_LEN];
    while (1) {
        int res;
        ipv6_addr_t src;

        size_t src_len = sizeof(ipv6_addr_t);
		memset(recv_buffer, 0, sizeof(recv_buffer));
		if((res = conn_udp_recvfrom(&conn, &recv_buffer, sizeof(recv_buffer),
									&src, &src_len, &port)) < 0) {
			printf("[CONN] Error while receiving, error code = %d\n", res);			

			// TODO error handling
        } else if(res == 0) {
            puts("[CONN] No data received\n");
            // TODO error handling
        } else {
			ipv6_addr_to_str(src_str, &src, IPV6_ADDR_MAX_STR_LEN);

			if (strcmp(last_src, src_str) == 0 && xtimer_now().ticks32 - last_src_rcv_time < RECV_DROP_DIFF_TIME){
#ifdef HAF_DEBUG
				printf("[CONN] PACKET DROPPED FROM UDP SERVER\n");
#endif
				continue;
			}
#ifdef HAF_DEBUG
			printf("[CONN] RECEIVED UDP PKG FROM %s\n", src_str);
#endif
            cb(recv_buffer, &src);
            last_src_rcv_time = xtimer_now().ticks32;
            memcpy(&last_src, &src_str, sizeof(src_str));
        }
    }
}



int udp_send(void* p, size_t p_size, ipv6_addr_t* dst){
	int res;
    ipv6_addr_t src = IPV6_ADDR_UNSPECIFIED;
    ipv6_addr_t d = IPV6_ADDR_UNSPECIFIED;
#ifdef HAF_DEBUG
	char src_str[IPV6_ADDR_MAX_STR_LEN];
#endif

    if (dst != NULL){
		//memcpy(&d, dst, sizeof(*dst));
		d = *dst;
	}else{
		if (ipv6_addr_from_str(&d, UDP_MULTICAST_ADDRESS) == NULL) {
			return -1;
		}
	}

    xtimer_usleep(NODE_ID * SEND_SLEEP_TIMER);

#ifdef HAF_DEBUG
    ipv6_addr_to_str(src_str, &d, IPV6_ADDR_MAX_STR_LEN);
    printf("[CONN] SEND UDP PKG TO %s\n", src_str);
#endif

    res = conn_udp_sendto(p, p_size, &src, sizeof(src), &d, sizeof(*dst), AF_INET6, UDP_SRC_PORT, UDP_RECV_PORT);

#ifdef HAF_DEBUG
    if (res < 0)
		printf("[CONN] UDP SEND ERROR %d\n", res);
#endif

    return res;
}
#endif

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

void get_ipv6_addr(ipv6_addr_t* addr){
	kernel_pid_t dev = _get_netif();
	gnrc_ipv6_netif_t *entry = gnrc_ipv6_netif_get(dev);

    memcpy(addr, &entry->addrs[1].addr, sizeof(ipv6_addr_t));
}

void setMonitorIP(ipv6_addr_t* monitorIP){
	memcpy(&monitor_ip,monitorIP,sizeof(ipv6_addr_t));
}

ipv6_addr_t* getMonitorIP(void){
	return &monitor_ip;
}