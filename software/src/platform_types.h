/* By default, an integer constant is treated as an int with the attendant limitations in values.
 * To specify an integer constant with another data type, follow it with: */

/* Add 'u' or 'U' to force a constant into an unsigned data format: Example 33u */
/* Add 'l' or 'L' to force the constant into a long data format. Example: 100000L */
/* Add 'ul' or 'UL' to force the constant into an unsigned long constant. Example: 32767ul */

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

typedef char 	int8_t;
typedef int		int16_t;
typedef long	int32_t;

typedef unsigned char 	uint8_t
typedef unsigned int	uint16_t;
typedef unsigned long	uint32_t;

#endif
