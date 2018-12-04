#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "vbe_macros.h"
#include "video_gr.h"

static uint16_t h_res;		//screen horizontal resolution
static uint16_t v_res; 		//screen vertical resolution
static uint8_t bits_per_pixel; // number of bits on each pixel
static char *video_mem;
static char *drawingBuffer;
static uint8_t 	redMaskSize;
static uint8_t 	redFieldPosition;
static uint8_t 	greenMaskSize;
static uint8_t 	greenFieldPosition;
static uint8_t 	blueMaskSize;
static uint8_t 	blueFieldPosition;
static uint16_t graphic_mode;

int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t* vmi_p){
   mmap_t map;
  phys_bytes buf;
    struct reg86u r;

  memset(&r, 0, sizeof(r));	/* zero the structure */


  if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
  	printf("vbe_get_mode_inf: failed to allocate memory \n");
  	return 1;
  }

buf = map.phys;

  r.u.w.ax = VBE_MODE_INFO; /* VBE get mode info */
  /* translate the buffer linear address to a far pointer */
  r.u.w.es = PB2BASE(buf); /* set a segment base */
  r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
  r.u.w.cx = mode;
  r.u.b.intno = VBE_INT;

  if (sys_int86(&r) != OK){
  	printf("vbe_get_mode_inf: sys_int86() failed to obtain mode info \n");
  	lm_free(&map);
  	return 1;
  }

  if(r.u.b.ah != 0){
  	lm_free(&map);
  	return 1;
  }

  *vmi_p = *(vbe_mode_info_t *)map.virt;

  lm_free(&map);

  return 0;
}

void *(vg_init)(uint16_t mode){
	graphic_mode = mode;

	if (lm_init(1) == NULL) {
  		printf("vbe_get_mode_inf: failed to initialize low memory \n");
  		return NULL;
 	}

	vbe_mode_info_t vbe;
	if (vbe_get_mode_inf(mode, &vbe) != OK){
		return NULL;
	}

	bits_per_pixel = vbe.BitsPerPixel;
	h_res = vbe.XResolution;
	v_res = vbe.YResolution;
	redMaskSize = vbe.RedMaskSize;
	redFieldPosition = vbe.RedFieldPosition;
	greenMaskSize = vbe.GreenMaskSize;
	greenFieldPosition = vbe.GreenFieldPosition;
	blueMaskSize = vbe.BlueMaskSize;
	blueFieldPosition = vbe.BlueFieldPosition;

	int r;
	struct minix_mem_range mr;  //struct to manage physical and virtual memory adresses
	unsigned int vram_base = (unsigned int) vbe.PhysBasePtr; //VRAM's physical address
	unsigned int vram_size = h_res*v_res*ceil(bits_per_pixel/8); //VRAM's size

	mr.mr_base = (phys_bytes) vram_base; //assigning the memory info to the struct
	mr.mr_limit = mr.mr_base + vram_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	panic("sys_privctl (ADD_MEM) failed: %d\n", r); /* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  drawingBuffer = (char*) malloc(h_res*v_res*bits_per_pixel/8);

	if(video_mem == MAP_FAILED)
		panic("couldn't map video memory");

	struct reg86u reg86;
	memset(&reg86, 0, sizeof(reg86));

	reg86.u.w.ax = SET_VBE; // VBE call, function 02 -- set VBE mode
	reg86.u.w.bx = BIT(14)|mode; // set bit 14: linear framebuffer
	reg86.u.b.intno = VBE_INT;
	if( sys_int86(&reg86) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	return video_mem;
}


int col(uint16_t x, uint16_t y, uint32_t color){
	if(x > h_res || y > v_res){
		printf("Error: that pixel does not exist!", 0);
		return 1;
	}
	int bytes_pixel = bits_per_pixel / 8;

	if (bits_per_pixel == 15){
		bytes_pixel = 2;
	}
	uint32_t col_tmp = color;
	int video_tmp = 0;

	for(int i = 0; i < bytes_pixel; i++, video_tmp++){
		video_mem [(y*h_res*bytes_pixel) + (x*bytes_pixel) + video_tmp] = col_tmp;
		col_tmp = col_tmp >> 8;
	}
	return 0;
}

int (vg_draw_hline)	(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
	for(int i = 0; i < len; x++,i++){
		if (x >= h_res ){
			continue;
		}
		if(col(x, y ,color) != OK){
			return 1;
		}
	}
	return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint32_t color){

	for(int i = 0; i< height; y++,i++){
		if (y  >= v_res ){
			break;
		}
		if(vg_draw_hline(x,y, width, color)!= OK){
			return 1;
		}
	}

	return 0;
}

int drawPattern (uint8_t no_rectangles, uint32_t first, uint8_t step){
	int rec_width = h_res / no_rectangles;
	int rec_height = v_res / no_rectangles;
	uint32_t red_mask, blue_mask, green_mask, R, G, B;

	static int color;

	for (unsigned int y = 0, i = 0; i < no_rectangles; i++, y += rec_height){
		for (unsigned int x = 0, j = 0; j < no_rectangles; j++, x += rec_width){

			if (graphic_mode == MODE105)
				color = (first + (i * no_rectangles + j) * step) % (1 << bits_per_pixel);
			else{
				if (graphic_mode == MODE110){
					red_mask = MASK_110_11A;
					green_mask = MASK_110_11A;
					blue_mask = MASK_110_11A;
				}else if (graphic_mode == MODE115 || graphic_mode == MODE14C){
					red_mask = MASK_115_14C;
					green_mask = MASK_115_14C;
					blue_mask = MASK_115_14C;
				}else if (graphic_mode == MODE11A){
					red_mask = MASK_110_11A;
					green_mask = GREEN_MASK_11A;
					blue_mask = MASK_110_11A;
				}else {
					return 1;
				}

				R = (((first >> redFieldPosition) & red_mask) + j * step) % (1 << redMaskSize);

          		G = (((first >> greenFieldPosition) & green_mask) + i * step) % (1 << greenMaskSize);

         		B = (((first >> blueFieldPosition) & blue_mask) + (i + j) * step) % (1 << blueMaskSize);

         		color = ((R << redFieldPosition) | (G << greenFieldPosition) | (B << blueFieldPosition));
			}

			if (vg_draw_rectangle(x, y, rec_width, rec_height, color) != OK){
				return 1;
			}

		}
	}


	return 0;
}

/*int drawSprite (const char *xpm[], uint16_t x, uint16_t y){
	uint16_t prev_x = x;
	int width;
	int height;
	char* sprite_addr;

	sprite_addr = read_xpm (xpm, &width, &height);

	for (int i = 0; i < height; i++,y++){
		if (y  >= v_res ){
				break;
		}
		for(int j = 0; j < width; j++, prev_x++){
			if (prev_x >= h_res ){
				continue;
			}
			if (col(prev_x, y, *(sprite_addr + i *width + j)) != OK){
				return 1;
			}
		}
		prev_x = x;
	}
	return 0;
}

void draw_sprite(Sprite *sp) {
  uint32_t color;
  int height = sp->height, width = sp->width, a = 0, y = sp->y, x = sp->x;

  for (int i = 0; i < height; i++, y++) {
    for (int j = 0; j < width; j++, x++) {
      color = sp->map[a];

      col(x, y, color);

      a++;
    }
    x = sp->x;
  }

}

void erase_sprite(Sprite *sp) {
  int height = sp->height, width = sp->width, y = sp->y, x = sp->x;

  for (int i = 0; i < height; i++, y++) {
    for (int j = 0; j < width; j++, x++) {
    	col(x, y, BACKGROUNDCOLOR);
    }
  	x = sp->x;
  }
}

void move_sprite(Sprite *sprite, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed){
	// if the speed is negative
	if (speed < 0) {
     	if (sprite->x < xf && yi == yf) {
            sprite->x++;
            erase_sprite(sprite);
            draw_sprite(sprite);

        } else if (xi == xf && sprite->y < yf) {
        	erase_sprite(sprite);
            sprite->y++;
            draw_sprite(sprite);

        } else if (sprite->x > xf && yi == yf) {
            erase_sprite(sprite);
            sprite->x--;
            draw_sprite(sprite);

        } else if (xi == xf && sprite->y > yf) {
            erase_sprite(sprite);
            sprite->y--;
            draw_sprite(sprite);
        }

    // if speed is positive
    } else {

        if (sprite->x < xf && yi == yf) {
            erase_sprite(sprite);
            if ((sprite->x + speed) > xf){
        		sprite->x = xf;
        	}
            sprite->x += sprite->xspeed;
            draw_sprite(sprite);
        }

        else if (xi == xf && sprite->y < yf) {
        	erase_sprite(sprite);
        	if ((sprite->y + speed) > yf){
        		sprite->y = yf;
        	}
            sprite->y += sprite->yspeed;
            draw_sprite(sprite);
        }

        else if (sprite->x > xf && yi == yf) {
            erase_sprite(sprite);
            if ((sprite->x + speed) > xf){
        		sprite->x = xf;
        	}
            sprite->x += -1 * sprite->xspeed;
            draw_sprite(sprite);
        }

        else if (xi == xf && sprite->y > yf) {
            erase_sprite(sprite);
            if ((sprite->y + speed) > yf){
        		sprite->y = yf;
        	}
        	sprite->y += -1 * sprite->yspeed;
            draw_sprite(sprite);
        }
    }
}*/

int vbe_get_controller_info(){

  struct reg86u r;
  mmap_t map;
  phys_bytes buf;
  vg_vbe_contr_info_t contr_info;


  if (  lm_init(1) == NULL) {
  		printf("vbe_get_controller_info: failed to initialize low memory \n");
  		return 1;
  }
  strcpy(contr_info.VBESignature, "VBE2");
  memset(&r, 0, sizeof(r));	/* zero the structure */

  if (lm_alloc(sizeof( contr_info), &map) == NULL) {
  	printf("vbe_get_controller_info: failed to allocate memory \n");
  	return 1;
  }

  buf = map.phys;

  r.u.w.ax = VBE_INFO;
  /* translate the buffer linear address to a far pointer */
  r.u.w.es = PB2BASE(buf); /* set a segment base */
  r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
  r.u.b.intno = VBE_INT;

  if (sys_int86(&r) != OK){
  	printf("vbe_get_controller_info: sys_int86() failed to obtain mode info \n");
  	lm_free(&map);
  	return 1;
  }


  contr_info = *((vg_vbe_contr_info_t*)map.virt);

  if (vg_display_vbe_contr_info (&contr_info) != OK){
  	printf("vbe_get_controller_info: failed displaying VBE controller information \n");
  	return 1;
  }

  lm_free(&map);

  return 0;
}

void doubleBuffCall(){
  memcpy(video_mem,drawingBuffer,h_res*v_res*bits_per_pixel/8);
}

int getHorResolution(){
  return (int)v_res;
}
int getVerResolution(){
  return (int)h_res;
}
char *getGraphicsBuffer(){
  return (char*)drawingBuffer;
}
