#include <stdio.h>

#include "board.h"
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#include "global.h"
#include "haf_LED.h"

/* 
 * Time for which blinking thread blinks.
 */
#define BLINK_TIME_USEC		3000000
/* 
 * Time in which LED is toggled on and off.
 */
#define BLINK_FREQ_USEC		 250000

static char stack[THREAD_STACKSIZE_DEFAULT];

/* 
 * Thread for blinking LED.
 * 
 * LED0_TOGGLE	=	RED
 * LED1_TOGGLE	=	GREEN
 * LED2_TOGGLE	=	BLUE
 */
void* _LED_blink(void* args) {
	int i;
	for(i = 0; i < BLINK_TIME_USEC / BLINK_FREQ_USEC; i++) {
		LED2_TOGGLE;
		xtimer_usleep(BLINK_FREQ_USEC);
	}
	LED2_OFF;
	return NULL;
}

/* 
 * Starts thread for blinking LED.
 */
void start_LED_blink(void) {
	thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 3, THREAD_CREATE_STACKTEST, 
						_LED_blink, NULL, "_LED_blink");	
}
