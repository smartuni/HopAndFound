#include <stdio.h>
#include <string.h>

#include "global.h"
#include "xtimer.h"
#include "connection.h"
#include "dispatcher.h"
#include "haf_button.h"
#include "haf_LED.h"
#include "localization_request.h"
#include "heartbeat.h"
#include "display.h"
#include "call_for_help.h"


int main(void){
	xtimer_sleep(STARTUP_SLEEPTIME_SEC);
	set_netif(POWER, SIGNAL_STRENGTH_MONITOR);
	set_netif(CHANNEL, NETIF_CHANNEL);
	
	xtimer_init();
	call_for_help_handler_init();
	displayInit();	
	
#ifdef HAF_DEBUG
	puts("Monitor start!");
#endif

#ifdef TEST_PRESENTATION
	init_LED();
	haf_button_init(haf_button_cb_monitor);
#endif

<<<<<<< HEAD
	call_for_help_handler_init();
	displayInit();
	printDisplayHopAndFound();
	
	udp_server_start((dispatcher_callback_t)dispatch_monitor);
		
	heartbeat_sender_start();
=======
	printDisplayHopAndFound();
	heartbeat_sender_init();
	udp_server_start((dispatcher_callback_t) dispatch_monitor);
>>>>>>> dev
	

	return 0;
}
