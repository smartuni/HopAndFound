#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"
#include "dispatcher.h"
#include "heartbeat.h"


#include "call_for_help.h" 


int main(void){
	
	xtimer_sleep(1);
	set_netif(POWER, -21);
	set_netif(CHANNEL, 11);
	
	puts("Monitored Item start!");

	udp_server_start((dispatcher_callback_t)dispatch_monitored_item);
	
	puts("Monitored Item Server up");
	
	return 0;
}
