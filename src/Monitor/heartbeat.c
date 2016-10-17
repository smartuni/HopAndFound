#include "xtimer.h"
#include "timex.h"
#include "net/gnrc.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/pktdump.h"

#define HEARTBEAT_TIME_USEC	3000000

/*
 * Timer.
 */
xtimer_t timer;

/*
 * Callback function of the timer.
 */
 
static void send(char *addr_str, char *port_str, char *data, unsigned int num,
                 unsigned int delay)
{
    uint16_t port;
    ipv6_addr_t addr;

    /* parse destination address */
    if (ipv6_addr_from_str(&addr, addr_str) == NULL) {
        puts("Error: unable to parse destination address");
        return;
    }
    /* parse port */
    port = (uint16_t)atoi(port_str);
    if (port == 0) {
        puts("Error: unable to parse destination port");
        return;
    }

    for (unsigned int i = 0; i < num; i++) {
        gnrc_pktsnip_t *payload, *udp, *ip;
        unsigned payload_size;
        /* allocate payload */
        payload = gnrc_pktbuf_add(NULL, data, strlen(data), GNRC_NETTYPE_UNDEF);
        if (payload == NULL) {
            puts("Error: unable to copy data to packet buffer");
            return;
        }
        /* store size for output */
        payload_size = (unsigned)payload->size;
        /* allocate UDP header, set source port := destination port */
        udp = gnrc_udp_hdr_build(payload, port, port);
        if (udp == NULL) {
            puts("Error: unable to allocate UDP header");
            gnrc_pktbuf_release(payload);
            return;
        }
        /* allocate IPv6 header */
        ip = gnrc_ipv6_hdr_build(udp, NULL, &addr);
        if (ip == NULL) {
            puts("Error: unable to allocate IPv6 header");
            gnrc_pktbuf_release(udp);
            return;
        }
        /* send packet */
        if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
            puts("Error: unable to locate UDP thread");
            gnrc_pktbuf_release(ip);
            return;
        }
        /* access to `payload` was implicitly given up with the send operation above
         * => use temporary variable for output */
        printf("Success: send %u byte to [%s]:%u\n", payload_size, addr_str,
               port);
        xtimer_usleep(delay);
    }
}

void timerTask(void) {
	//TODO: Send
	send("ff02::1", "8888", "HEARTBEAT", 1, 1000000);
	xtimer_set(&timer, HEARTBEAT_TIME_USEC);
}

void heartbeatStart(void){
	xtimer_init();

    timer.target = 0;
    timer.long_target = 0;
    timer.callback = (void*)timerTask;

    timerTask();
}
