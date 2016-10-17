
#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "heartbeat.h"

/*
 * Main function.
 */
int main(void)
{
    puts("This is Heartbeat - Monitor");

	heartbeatStart();

    while(1){}

    return 0;
}

