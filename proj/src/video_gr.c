#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>

#include "video_gr.h"
#include "vbe_macros.h"

static uint16_t h_res;		//screen horizontal resolution
static uint16_t v_res; 		//screen vertical resolution
static uint8_t bits_per_pixel; // number of bits on each pixel
static char *video_mem;         //main buffer
static char *drawingBuffer;     //second buffer

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


void doubleBuffCall(){
  memcpy(video_mem,drawingBuffer,h_res*v_res*bits_per_pixel/8);
}

int getHorResolution(){
  return (int)h_res;
}
int getVerResolution(){
  return (int)v_res;
}
unsigned char *getGraphicsBuffer(){
  return (unsigned char*)drawingBuffer;
}
