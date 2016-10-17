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

static gnrc_netreg_entry_t server = { NULL, GNRC_NETREG_DEMUX_CTX_ALL, KERNEL_PID_UNDEF };


static void start_server(char *port_str)
{
    uint16_t port;

    /* check if server is already running */
    if (server.pid != KERNEL_PID_UNDEF) {
        printf("Error: server already running on port %" PRIu32 "\n",
               server.demux_ctx);
        return;
    }
    /* parse port */
    port = (uint16_t)atoi(port_str);
    if (port == 0) {
        puts("Error: invalid port specified");
        return;
    }
    /* start server (which means registering pktdump for the chosen port) */
    server.pid = gnrc_pktdump_pid;
    server.demux_ctx = (uint32_t)port;
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &server);
    printf("Success: started UDP server on port %" PRIu16 "\n", port);
}


/*
 * Callback function of the timer.
 */
void timerTask(void) {
	//TODO: Send
	xtimer_set(&timer, HEARTBEAT_TIME_USEC);
}

void heartbeatStart(void){
	
	xtimer_init();
	
	start_server("8888");
	
    timer.target = 0;
    timer.long_target = 0;
    timer.callback = (void*)timerTask;

    timerTask();
}
