#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "global.h"

#define CHAR_WIDTH          (6U)
#define ROW_AMOUNT 			(3)
#define MAP_SIZE			(9*ROW_AMOUNT*5)      
#define MAP_X_OFFSET		(4)
#define MAP_Y_OFFSET		(1)

#define DEVICE_X_OFFSET		(1)
#define DEVICE_Y_OFFSET		(ROW_AMOUNT+MAP_Y_OFFSET+1)

static pcd8544_t dev;

static const char map_node[MAP_SIZE] = {
	//first row 1---2---3
#define DISPLAY_NODE_1    0x3e, 0x51, 0x49, 0x45, 0x3e,
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

static const char map[MAP_SIZE] = {
	//first row X---X---X
    0x63, 0x14, 0x08, 0x14, 0x63,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x63, 0x14, 0x08, 0x14, 0x63,
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

	//third row X---X---X
    0x63, 0x14, 0x08, 0x14, 0x63,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x63, 0x14, 0x08, 0x14, 0x63,
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
	pcd8544_write_s(&dev,DEVICE_X_OFFSET,DEVICE_Y_OFFSET,"Device lost!");
}

void printMonitoredItemInRange(void){   
	pcd8544_write_s(&dev,DEVICE_X_OFFSET,DEVICE_Y_OFFSET,"Device in range!");
}

void printDisplayHopAndFound(void){
	 pcd8544_clear(&dev);

	 pcd8544_write_s(&dev,1,2,"Hop And Found"); 
}

void printDisplayHopAndFoundActive(void){
	 pcd8544_clear(&dev);

	 pcd8544_write_s(&dev,0,1,"Hop And Found");
     pcd8544_write_s(&dev,3,3,"active!");  
}

void printDisplayMap(void){
   pcd8544_clear(&dev);
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


void printDisplayMapString(uint8_t* nodes){
   pcd8544_clear(&dev);

   char row1[8]="X--X--X";
   char row3[8]="X--X--X";
   
   if(nodes[0]==1){
	  row1[0]='0';
   }
   if(nodes[1]==1){
	  row1[3]='1';
   }
   if(nodes[2]==1){
	  row1[6]='2';
   }
   if(nodes[3]==1){
	  row3[0]='3';
   }
   if(nodes[4]==1){
	  row3[3]='4';
   }   
   if(nodes[5]==1){
	  row3[6]='5';
   }
   
   pcd8544_write_s(&dev,MAP_X_OFFSET,MAP_Y_OFFSET,row1);
   pcd8544_write_s(&dev,MAP_X_OFFSET,MAP_Y_OFFSET+1,"|     |");
   pcd8544_write_s(&dev,MAP_X_OFFSET,MAP_Y_OFFSET+2,row3);
   
   printMonitoredItemLost();
}


void printDisplayMapStringPath(uint8_t* nodes,uint8_t* node_list_path){
   pcd8544_clear(&dev);
#ifdef HAF_DEBUG

	puts("Path:");
   	for(int i = 0; i < MAX_NODES; i++) {
		printf("node_list[%d]: %u\n", i,node_list_path[i]);
	}
#endif
   char row1[10]="X   X   X";
   char row2[10]="         "; //row 3 always the same as row2
   char row4[10]="X   X   X";
   
   if(nodes[0]==1){
	  row1[0]='0';
   }
   if(nodes[1]==1){
	  row1[4]='1';
   }
   if(nodes[2]==1){
	  row1[8]='2';
   }
   if(nodes[3]==1){
	  row4[0]='3';
   }
   if(nodes[4]==1){
	  row4[4]='4';
   }   
   if(nodes[5]==1){
	  row4[8]='5';
   }
   
   if(node_list_path[0] && node_list_path[1]){
	   row1[1]='-';
	   row1[2]='-';
	   row1[3]='-';
   }
   if(node_list_path[1] && node_list_path[2]){
	   row1[5]='-';
	   row1[6]='-';
	   row1[7]='-';
   }
   if(node_list_path[3] && node_list_path[4]){
	   row4[1]='-';
	   row4[2]='-';
	   row4[3]='-';
   }
   if(node_list_path[4] && node_list_path[5]){
	   row4[5]='-';
	   row4[6]='-';
	   row4[7]='-';
   }
   if(node_list_path[0] && node_list_path[3]){
	  row2[0]='|';  
   }
   if(node_list_path[2] && node_list_path[5]){
	  row2[9]='|';  
   }
      if(node_list_path[1] && node_list_path[4]){
	  row2[4]='|';  
   }
   
   pcd8544_write_s(&dev,MAP_X_OFFSET/2,MAP_Y_OFFSET,row1);
   pcd8544_write_s(&dev,MAP_X_OFFSET/2,MAP_Y_OFFSET+1,row2);
   pcd8544_write_s(&dev,MAP_X_OFFSET/2,MAP_Y_OFFSET+2,row2);
   pcd8544_write_s(&dev,MAP_X_OFFSET/2,MAP_Y_OFFSET+3,row4);

}




void displayInit(void){
	printf("%d , %d\n",TEST_PCD8544_CS,TEST_PCD8544_MODE);

    printf("Initializing PCD8544 LCD at SPI_%i... ", TEST_PCD8544_SPI);//TEST_X defined in MAKEFILE
    if (pcd8544_init(&dev, TEST_PCD8544_SPI, TEST_PCD8544_CS,
                     TEST_PCD8544_RESET, TEST_PCD8544_MODE) != 0) {
        puts("Failed to initialize PCD8544 display");
    }
    puts("Display initialized!");
}
