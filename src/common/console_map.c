#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "console_map.h"
#include "xtimer.h"

#define INACTIVE_NODE_CHARACTER ('X')
#define ACTIVE_NODE_CHARACTER ('O')

static char* map_template = "\
		\x1b[00mX\x1b[0m-----------\x1b[00mX\x1b[0m-----------\x1b[00mX\x1b[0m\n\
		|                       |\n\
		|                       |\n\
		|                       |\n\
		\x1b[00mX\x1b[0m-----------\x1b[00mX\x1b[0m-----------\x1b[00mX\x1b[0m\n";


void generateAndPrintRandomMaps(void){
	uint8_t nodes[6];
	int i;
	
	xtimer_sleep(1);
	
	while(1){
		printf("Nodes: ");
		for (i = 0; i < sizeof(nodes); i++){
			nodes[i] = rand() % 2;
			printf("%d", nodes[i]);
		}
		printf("\n");
		printConsoleMap(nodes, sizeof(nodes));
		xtimer_sleep(1);
	}
}


void highlight_nodes_in_map(char* string, uint8_t* occurances, size_t len){
	unsigned int i;
	unsigned int occurance_count;
	
	occurance_count = 0;
	for(i = 0; i < strlen(string); i++){
		if (string[i] == INACTIVE_NODE_CHARACTER){
			if (occurance_count >= len){
				return;
			}
			if (occurances[occurance_count] == 1){
				// DIRTY: replace the color code in map_template from "normal" to "red"
				string[i-3] = '3';
				string[i-2] = '1';
				
				string[i] = ACTIVE_NODE_CHARACTER;
			}
			occurance_count++;
		}
	}
}

void printConsoleMap(uint8_t* nodes, size_t len){
	char* map_to_print = (char*) malloc(strlen(map_template) + 1);
	
	strcpy(map_to_print, map_template);
	
	highlight_nodes_in_map(map_to_print, nodes, len);
	
	printf("\n%s\n", map_to_print);
	
	free(map_to_print);
}
