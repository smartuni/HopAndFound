#include <stdio.h>
#include <string.h>

#include "global.h"
#include "xtimer.h"
#include "dispatcher.h"
#include "haf_button.h"
#include "haf_LED.h"
#include "localization_request.h"
#include "heartbeat.h"
#include "routing.h"
#include "call_for_help.h"
#include "connection.h"
#include "haf_thread.h"


int main(void){
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER, SIGNAL_STRENGTH_MONITORED_ITEM);
	set_netif(CHANNEL, NETIF_CHANNEL);

#ifdef HAF_DEBUG
	puts("Monitored Item start!");
	ipv6_addr_t *addr = (ipv6_addr_t*) malloc(sizeof(ipv6_addr_t));
	char src_str[IPV6_ADDR_MAX_STR_LEN];
	get_ipv6_addr(addr);
	ipv6_addr_to_str(src_str, addr, IPV6_ADDR_MAX_STR_LEN);
	printf("Ipv6 Address = %s\n", src_str);
	free(addr);

#ifdef HAF_USE_SOCK_UDP
	puts("Using sock_udp");
#else
	puts("Using conn_udp");
#endif
#endif

	xtimer_init();
	haf_thread_create();
#ifdef TEST_PRESENTATION
	init_LED();
	haf_button_init(haf_button_cb_monitored_item);
	localization_request_init(localization_request_cb_monitored_item);
#endif
	heartbeat_timeout_init();

	//heartbeat_timeout_init(); // Move to bind.c  handle_bind(); 
	udp_server_start((dispatcher_callback_t) dispatch_monitored_item);

	return 0;
}



