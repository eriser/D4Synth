#ifndef _LOOKUP_H_
#define _LOOKUP_H_

#include <stdbool.h>

#include <avr/pgmspace.h>
#include "op.h"

typedef uint8_t prog_uint8_t;
typedef uint16_t prog_uint16_t;

inline uint16_t readWord16(const prog_uint16_t* source, const uint16_t index)
{
	return pgm_read_word(source + index);
}

inline uint16_t readAndInterpolateWord16(const prog_uint16_t* source, 
												const uint16_t indexA, 
												const uint16_t indexB,
												const uint8_t position)
{
	uint16_t a = readWord16(source, indexA);
	uint16_t b = readWord16(source, indexB);
	uint16_t partA = U16U8MulShift8(a, 0xFF - position);
	uint16_t partB = U16U8MulShift8(b, position);
	return partA + partB;
}

#endif
