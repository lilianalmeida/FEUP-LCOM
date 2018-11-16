#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "macros.h"

static unsigned h_res;		//screen horizontal resolution
static unsigned v_res; 		//screen vertical resolution
static unsigned bits_per_pixel; // number of bits on each pixel
static char *video_mem;

int (vg_initialize)(uint16_t mode){

	struct reg86u reg86;
	memset(&reg86, 0, sizeof(reg86));

	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1<<14|0x105; // set bit 14: linear framebuffer
	reg86.u.b.intno = 0x10;
	if( sys_int86(&reg86) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return 1;
	}

	vbe_mode_info_t vbe;
	if (vbe_get_mode_info(mode, &vbe) != OK){
		return 1;
	}

	h_res = vbe.XResolution;
	v_res = vbe.YResolution;
	bits_per_pixel = vbe.BitsPerPixel;

	struct minix_mem_range mr;  //struct to manage physical and virtual memory adresses
	unsigned int vram_base = (unsigned int) vbe.PhysBasePtr; //VRAM's physical address
	unsigned int vram_size = h_res*v_res*(bits_per_pixel/8); //VRAM's size

	mr.mr_base = (phys_bytes) vram_base; //assigning the memory info to the struct
	mr.mr_limit = mr.mr_base + vram_size;

	int r;
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	panic("sys_privctl (ADD_MEM) failed: %d\n", r); /* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
	panic("couldn't map video memory");

	return 0;
}


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
