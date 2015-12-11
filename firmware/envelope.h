#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <stdio.h>
#include <ctype.h>

typedef enum {
	attack = 0,
	decay = 1,
	sustain = 2, // stage where nothing happens, after decay is over
	release = 3,
	dead = 4
} envStage_t;

typedef struct {
	uint32_t phase_;
	uint16_t S_;
} envelope_t;


void env_init(envelope_t env);
void env_setA(envelope_t env, uint16_t A);
void env_setD(envelope_t env, uint16_t D);
void env_setS(envelope_t env, uint16_t S);
void env_setR(envelope_t env, uint16_t R);
void triggerStage(envelope_t env, envStage_t stage);



#endif
