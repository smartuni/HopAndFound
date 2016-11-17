#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "global.h"

#define CHAR_WIDTH          (6U)
#define ROW_AMOUNT 			(3)
#define MAP_SIZE			(9*ROW_AMOUNT*5)      
#define MAP_X_OFFSET		20
#define MAP_Y_OFFSET		2

#define DEVICE_X_OFFSET		1
#define DEVICE_Y_OFFSET		ROW_AMOUNT+MAP_Y_OFFSET

static pcd8544_t dev;

static const char map[MAP_SIZE] = {
	//first row 1---2---3
    0x3e, 0x51, 0x49, 0x45, 0x3e,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x42, 0x7f, 0x40, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x42, 0x61, 0x51, 0x49, 0x46,
	//second row |       |
	0x00, 0x00, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7f, 0x00, 0x00,

	//third row 3---4---5
    0x21, 0x41, 0x45, 0x4b, 0x31,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x18, 0x14, 0x12, 0x7f, 0x10,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x27, 0x45, 0x45, 0x45, 0x39,
};

static void _write(pcd8544_t *dev, uint8_t is_data, char data)
{
    /* set command or data mode */
    gpio_write(dev->mode, is_data);
    /* write byte to LCD */
    spi_acquire(dev->spi);
    gpio_clear(dev->cs);
    spi_transfer_byte(dev->spi, data, 0);
    gpio_set(dev->cs);
    spi_release(dev->spi);
}

static inline void _set_x(pcd8544_t *dev, uint8_t x)
{
    _write(dev, MODE_CMD, CMD_SET_X | x);
}

static inline void _set_y(pcd8544_t *dev, uint8_t y)
{
    _write(dev, MODE_CMD, CMD_SET_Y | y);
}

void printMonitoredItemLost(void){   
	pcd8544_write_s(&dev,0,DEVICE_Y_OFFSET,"Device lost!");
}

void printMonitoredItemInRange(void){   
	pcd8544_write_s(&dev,DEVICE_X_OFFSET,DEVICE_Y_OFFSET,"Device in range!");
}

void printDisplayMap(void){
   pcd8544_write_s(&dev,0,0,"Hop And Found");
   
   _set_y(&dev, MAP_Y_OFFSET);
   _set_x(&dev, MAP_X_OFFSET);
   for(int y=0; y < ROW_AMOUNT ; y++){
    for (int i = 0; i <(MAP_SIZE/ROW_AMOUNT); i++) {
        _write(&dev, MODE_DTA, map[i+(y*(MAP_SIZE/ROW_AMOUNT))]);
    }
	_set_y(&dev, y+1+MAP_Y_OFFSET);
	_set_x(&dev, MAP_X_OFFSET);
   }
   printMonitoredItemLost();
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
