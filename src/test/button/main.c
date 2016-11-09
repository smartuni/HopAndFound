#include <stdio.h>
#include <string.h>

#include "global.h"
#include "haf_button.h"

int main(void) {
	haf_button_init(haf_button_cb_node);
	
	while(1){}
	return 0;
}
