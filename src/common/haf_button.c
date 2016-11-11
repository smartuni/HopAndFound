#include <stdio.h>
#include <string.h>

#include "global.h"
#include "haf_button.h"
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "localization_request.h"
#include "heartbeat.h"

#define DEBOUNCE_TIME_USEC	250000

xtimer_t _timer_debounce;
bool _debounced = true;

void _debounce_cb(void) {
	_debounced = true;
}

/* 
 * LED0_TOGGLE	=	RED
 * LED1_TOGGLE	=	GREEN
 * LED2_TOGGLE	=	BLUE
 */
void haf_button_cb_node(void *arg) {
	if(_debounced) {
		_debounced = false;
		send_localization_request_node();
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

void haf_button_cb_monitor(void *arg) {
	if(_debounced) {
		_debounced = false;
		if(getHeartbeatActive()) {
			setHeartbeatActive(false);
			LED0_ON;
		} else {
			setHeartbeatActive(true);
			LED0_OFF;
		}
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

void haf_button_cb_monitored_item(void *arg) {
	if(_debounced) {
		_debounced = false;
		//TODO: add functionality
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

int haf_button_init(haf_button_cb_t cb) {
	xtimer_init();
    _timer_debounce.target = 0;
    _timer_debounce.long_target = 0;
    _timer_debounce.callback = (void*)_debounce_cb;
	if (gpio_init_int(BUTTON_GPIO, GPIO_IN_PU, GPIO_FALLING, cb, NULL) < 0) { 
		printf("error: gpio_init_int\n"); 
		return 1; 
	}
	return 0;
}
