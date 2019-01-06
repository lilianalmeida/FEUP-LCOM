#ifndef _VBE_MACROS_H_
#define _VBE_MACROS_H_

/** @defgroup Vbe_macros Vbe_macros
 * @{
 *
 * Constants for programming the vbe
 */

#define BIT(n) 		(0x01 << (n))
#define PB2BASE(x)	(((x) >> 4) &  0x0F000)	/**< @brief Computes the Base of a Segment */
 
#define SET_VBE						0x4F02	/**< @brief Set VBE Mode */
#define VBE_INT						0x10 	/**< @brief BIOS Video Services */
#define VBE_MODE_INFO				0x4F01	/**< @brief Get VBE Mode Information */

#define MODE117						0x117 	/**< @brief Graphics mode 117*/
/**@}*/
#endif /* _VBE_MACROS_H_ */
