#include <stdio.h>
#include <string.h>

#include "haf_button.h"
#include "board.h"
#include "periph/gpio.h"

static void cb(void *arg){ 
	haf_button_var++;
	LED2_TOGGLE;
	puts("Button pressed!");
}

int button_init(void){
	
	haf_button_var = 0;
	puts("button_init");
	printf("PIN: %i\n", BUTTON_PIN);

	if (gpio_init_int(BUTTON_GPIO, GPIO_IN_PU, GPIO_FALLING, cb, NULL) < 0) { 
		printf("error: gpio_init_int\n"); 
		return 1; 
	}
	
	return 0;
}
