#include <stdio.h>
#include <string.h>

#include "global.h"
#include "haf_button.h"
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "localization_request.h"
#include "heartbeat.h"
#include "bind.h"
#include "haf_queue.h"

#define DEBOUNCE_TIME_USEC	(250000)

haf_button_cb_t button_cb;
xtimer_t _timer_debounce;
bool _debounced = true;

void _debounce_cb(void) {
	_debounced = true;
}

void haf_button_cb_node(void) {
	if(_debounced) {
		_debounced = false;
		send_localization_request();
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

void haf_button_cb_monitor(void) {
	if(_debounced) {
		_debounced = false;
		if(getHeartbeatActive()) {
			setHeartbeatActive(false);
			LED2_ON;
		} else {
			setHeartbeatActive(true);
			LED2_OFF;
		}
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

void haf_button_cb_monitored_item(void) {
	if(_debounced) {
		_debounced = false;
		send_bind();
		xtimer_set(&_timer_debounce, DEBOUNCE_TIME_USEC);
	}
}

void gpio_interupt(void){
	if (haf_queue_enqueue((thread_function_t) button_cb) == -1){
#ifdef HAF_DEBUG
		printf("gpio_interupt:: too many elements in thread queue.\n");
#endif /* HAF_DEBUG */
	}
}

int haf_button_init(haf_button_cb_t cb) {
    _timer_debounce.target = 0;
    _timer_debounce.long_target = 0;
    _timer_debounce.callback = (void*)_debounce_cb;
	button_cb = cb;
	if (gpio_init_int(BUTTON_GPIO, GPIO_IN_PU, GPIO_FALLING, (void*) gpio_interupt, NULL) < 0) { 
		printf("error: gpio_init_int\n"); 
		return 1; 
	}
	return 0;
}
