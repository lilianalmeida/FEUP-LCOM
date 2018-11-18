#include <lcom/lcf.h>
#include "vbe_test.h"
#include "macros.h"

int (col)(uint16_t x, uint16_t y, uint32_t color){

	if(x > h_res || y > v_res){
		printf("Error: that pixel does not exist!", 0);
		return 1;
	}

	video_mem[ y*h_res + x] = color;

	return 0;
}

int (drawSquare)(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint32_t color){

	for(int i = y; i < height; i++){
		for(int j = x; i < width; j++){
			col(j+x, i+y ,color);
			}
		}

	return 0;
}