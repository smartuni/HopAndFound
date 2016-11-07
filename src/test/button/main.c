#include <stdio.h>
#include <string.h>

#include "global.h"
#include "haf_button.h"
#include "xtimer.h"

int main(void) {
	button_init();
	
	while(1){
		printf("var: %d\n", haf_button_var);
		xtimer_sleep(2);
	}
	
	return 0;
}
