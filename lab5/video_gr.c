#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "macros.h"

static uint16_t h_res;		//screen horizontal resolution
static uint16_t v_res; 		//screen vertical resolution
static uint8_t bits_per_pixel; // number of bits on each pixel
static char *video_mem;
static uint8_t 	redMaskSize;
static uint8_t 	redFieldPosition;
static uint8_t 	greenMaskSize;
static uint8_t 	greenFieldPosition;
static uint8_t 	blueMaskSize;
static uint8_t 	blueFieldPosition;

void *(vg_init)(uint16_t mode){
	lm_init(1);
	struct reg86u reg86;
	memset(&reg86, 0, sizeof(reg86));
	///por em constantes no macros.h estas variaveis todas
	reg86.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = 1<<14|0x105; // set bit 14: linear framebuffer
	reg86.u.b.intno = 0x10;
	if( sys_int86(&reg86) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	vbe_mode_info_t vbe;
	if (vbe_get_mode_info(mode, &vbe) != OK){
		return NULL;
	}

	h_res = vbe.XResolution;
	v_res = vbe.YResolution;
	redMaskSize = vbe.RedMaskSize;
	redFieldPosition = vbe.RedFieldPosition;
	greenMaskSize = vbe.GreenMaskSize;
	greenFieldPosition = vbe.GreenFieldPosition;
	blueMaskSize = vbe.BlueMaskSize;
	blueFieldPosition = vbe.BlueFieldPosition;

	bits_per_pixel = vbe.BitsPerPixel;

	struct minix_mem_range mr;  //struct to manage physical and virtual memory adresses
	phys_bytes vram_base = vbe.PhysBasePtr; //VRAM's physical address
	unsigned int vram_size = h_res*v_res*(bits_per_pixel/8); //VRAM's size

	mr.mr_base = vram_base; //assigning the memory info to the struct
	mr.mr_limit = mr.mr_base + vram_size;

	int r;
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	panic("sys_privctl (ADD_MEM) failed: %d\n", r); /* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

	if(video_mem == MAP_FAILED)
	panic("couldn't map video memory");

	return video_mem;
}

int (col)(uint16_t x, uint16_t y, uint32_t color){
	if(x > h_res || y > v_res){
		printf("Error: that pixel does not exist!", 0);
		return -1;
	}
	//conforme o modo de cores??
	video_mem [y*h_res*(bits_per_pixel / 8) + x*(bits_per_pixel / 8)] = color & 0xf;
	return 0;
}

int (vg_draw_hline)	(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
	for(int j = x; (j-x) < len; j++){
			if(col(j, y ,color) != OK){
				return -1;
			}
	}
	return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint32_t color){

	for(int i = y; (i-y) < height; i++){
		if(vg_draw_hline(x,i, width, color)!= OK){
			return -1;
		}
	}

	return 0;
}

int (drawPattern) (uint8_t no_rectangles, uint32_t first, uint8_t step){
	uint16_t rec_width = h_res / (uint16_t) no_rectangles;
	uint16_t rec_height = v_res / (uint16_t) no_rectangles;
	uint16_t x = 0, y = 0;
	uint32_t color = first;

	for (uint8_t i = 1; i <= no_rectangles; i++, y += rec_height){
		for (uint8_t j = 1; j <= no_rectangles; j++, x += rec_width){
			vg_draw_rectangle(x, y, rec_width, rec_height, color);

			//color in indexed mode
			color = (first + (x * no_rectangles + y) * step) % (1 << bits_per_pixel);
		}
		x = 0;
	}
	return 0;
}
int (drawSprite) (const char *xpm[], uint16_t x, uint16_t y){
	int width = 0;
	int height = 0;
	char* sprite_addr;
	sprite_addr = read_xpm (xpm, &width, &height);

	for (int i = 0; i < height; i++ )
		for(int j = 0; j < width; j++){
			col(x+j,y + i,*(sprite_addr + i *width + j));
		}

	return 0;
}
