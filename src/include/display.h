#ifndef DISPLAY_H
#define DISPLAY_H

#include "HAF_protocol.h"
#include "pcd8544.h"
#include "pcd8544_internal.h"

void printDisplayMap(void);
void printDisplayMapString(uint8_t* nodes);
void printDisplayMapStringPath(uint8_t* nodes,uint8_t* node_list_path);
void printDisplayHopAndFound(void);
void printDisplayHopAndFoundActive(void);
void displayInit(void);


#endif
