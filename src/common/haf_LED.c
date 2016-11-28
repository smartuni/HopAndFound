#include <stdio.h>

#include "board.h"
#include "periph/gpio.h"
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
 * Timer for LED toggle.
 */
xtimer_t blink_timer;
/* 
 * Current LED.
 */
led_t _led;
/* 
 * Time the LED blinks in seconds.
 */
uint _time;
/* 
 * LED toggle index.
 */
uint _index = 0;

/* 
 * Timer callback function for toggling LED.
 */
void _blink_timer_task(void) {
	if(_index < (_time * BLINK_MULTIPLIER_USEC / BLINK_FREQ_USEC)) {
		_index++;
		if(_led == LED_RED) {
			LED0_TOGGLE;
		} else if(_led == LED_GREEN) {
			LED1_TOGGLE;
		} else if(_led == LED_BLUE) {
			LED2_TOGGLE;
		}
		xtimer_set(&blink_timer, BLINK_FREQ_USEC);
	} else {
		_index = 0;
		if(_led == LED_RED) {
			LED0_OFF;
		} else if(_led == LED_GREEN) {
			LED1_OFF;
		} else if(_led == LED_BLUE) {
			LED2_OFF;
		}
	}
}

void init_LED(void) {
    blink_timer.target = 0;
    blink_timer.long_target = 0;
    blink_timer.callback = (void*)_blink_timer_task;
}

void start_LED_blink(led_t led, uint time) {
	_led = led;
	_time = time;
	xtimer_set(&blink_timer, BLINK_FREQ_USEC);
}
