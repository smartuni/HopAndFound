#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "shell.h"
#include "udp.h"

int main(void)
{
	int sn;
	TPacket p;
	
    if (udp_server_start() != 0)
    {
		printf("Failed to start udp server\n");
    }
    
    p.header = 1;
    p.payload = 42;
    
    sn = 0;
    while(1)
    {
		p.sequence_number = sn;
		udp_send(&p);
		xtimer_sleep(1);
    }

    return 0;
}
