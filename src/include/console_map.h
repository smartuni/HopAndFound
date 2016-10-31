#ifndef CONSOLE_MAP_H
#define CONSOLE_MAP_H

/*
  Prints a map on the console, highlighting the nodes give in first parameter.
  \param  nodes Array of nodes where 1 stands for "to be highligthed" and 0 for not. Example: {1, 1, 0, 1, 0, 0}
  \param len Length of nodes array.
*/
void printConsoleMap(uint8_t* nodes, size_t len); 

/*
  Generates random maps for 6 nodes and prints them in an infinite loop.
*/
void generateAndPrintRandomMaps(void);

#endif
