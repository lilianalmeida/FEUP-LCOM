#ifndef _VIDEO_GR_H_
#define _VIDEO_GR_H_

#include <stdint.h>

/** @defgroup Video_gr Video_gr
* @{
* Functions for using the vbe
*/

/**
 * @brief Returns information on the input VBE mode, including screen dimensions, color depth and VRAM physical address
 *
 * @param mode mode whose information should be returned
 * @param vmi_p	address of vbe_mode_info_t structure to be initialized
 * @return Return 0 upon success and non-zero otherwise
 */
int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t* vmi_p);

/**
 * @brief Copies to the main buffer (video_mem) what is in the other buffer (drawingBuffer)
 */
void doubleBuffCall();

/**
 * @brief Returns the horizontal resolution in pixels
 *
 * @return Return horizontal resolution
 */
int getHorResolution();

/**
 * @brief Returns the vertical resolution in pixels
 *
 * @return Return vertical resolution
 */
int getVerResolution();

/**
 * @brief Returns thea address of the second buffer (drawingBuffer)
 *
 * @return Return address of the graphics buffer
 */
unsigned char *getGraphicsBuffer();

/**@}*/

#endif
