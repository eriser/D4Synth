#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <stdbool.h>

#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

// stores pitches for midi notes [0-12) (excluding 12.0000)
// higher octaves are obtained by doubling (<< 1)
// index is: pitch * 64 (= pitch << 6)
// result is frequency * 512 (= frequency << 9)
const uint16_t pitchTable[768] PROGMEM;
#define PITCH_TABLE_HIGHEST	12
#define PITCH_TABLE_SIZE 768
#define PITCH_TABLE_LAST (PITCH_TABLE_SIZE - 1)

#ifdef __cplusplus
{
#endif

#endif
