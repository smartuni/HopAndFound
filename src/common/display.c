#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "global.h"


static pcd8544_t dev;


void printDisplayMap(void){
    pcd8544_write_s(&dev, 1, 1, "Hallo");
	pcd8544_riot(&dev);
}

void displayInit(void){
	printf("%d , %d",TEST_PCD8544_CS,TEST_PCD8544_MODE);
    printf("Initializing PCD8544 LCD at SPI_%i... ", TEST_PCD8544_SPI);//TEST_X defined in MAKEFILE
    if (pcd8544_init(&dev, TEST_PCD8544_SPI, TEST_PCD8544_CS,
                     TEST_PCD8544_RESET, TEST_PCD8544_MODE) != 0) {
        puts("Failed to initialize PCD8544 display");
    }
    puts("Display initialized!");
}
