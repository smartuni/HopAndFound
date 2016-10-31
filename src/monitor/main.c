#include <stdio.h>
#include <string.h>

#include "connection.h"
#include "xtimer.h"

int main(void){
	
	xtimer_sleep(1);
	set_netif(POWER, 11);
	puts("Sent Packet \n");



	return 0;
}
