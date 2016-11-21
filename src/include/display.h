#ifndef DISPLAY_H
#define DISPLAY_H

#include "HAF_protocol.h"
#include "connection.h"
#include "pcd8544.h"
#include "pcd8544_internal.h"

void printDisplayMap(void);
void printDisplayMapString(uint8_t* nodes);
void printDisplayHopAndFound(void);
void displayInit(void);


#endif
