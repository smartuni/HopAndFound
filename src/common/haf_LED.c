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
#define BLINK_MULTIPLIER_USEC	1000000
/* 
 * Time in which LED is toggled on and off.
 */
#define BLINK_FREQ_USEC			250000

/* 
 * Current LED.
 */
led_t _led;
/* 
 * Time the LED blinks in seconds.
 */
uint _time;

static char stack[THREAD_STACKSIZE_DEFAULT];

/* 
 * Thread for blinking LED.
 */
void* _LED_blink(void* args) {
	int i;
	for(i = 0; i < (_time * BLINK_MULTIPLIER_USEC) / BLINK_FREQ_USEC; i++) {
		if(_led == LED_RED) {
			LED0_TOGGLE;
		} else if(_led == LED_GREEN) {
			LED1_TOGGLE;
		} else if(_led == LED_BLUE) {
			LED2_TOGGLE;
		}
		
		xtimer_usleep(BLINK_FREQ_USEC);
	}
	return NULL;
}

/* 
 * Starts thread for blinking LED.
 */
void start_LED_blink(led_t led, uint time) {
	_led = led;
	_time = time;
	thread_create(stack, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 3, THREAD_CREATE_STACKTEST, 
						_LED_blink, NULL, "_LED_blink");	
}
