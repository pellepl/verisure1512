/*
 * common.h
 *
 *  Created on: Dec 8, 2015
 *      Author: petera
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef unsigned char bool;
typedef unsigned char u8_t;
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned int u32_t;
typedef signed int s32_t;
typedef unsigned long long u64_t;
typedef signed long long s64_t;
#define ABS(_x) ((_x)<0 ? (-(_x)) : (_x))
#define MIN(_x, _y) ((_x)>(_y) ? (_y) : (_x))
#define MAX(_x, _y) ((_x)<(_y) ? (_y) : (_x))
#define NULL ((void *)0)
#define FALSE (0)
#define TRUE  (!FALSE)
#define memcpy(d,s,n) __builtin_memcpy((d),(s),(n))
#define memset(s,t,n) __builtin_memset((s),(t),(n))

#define IOSTD   (0)

extern void *_variadic_call(void *func, int argc, void* args);
void halt(const char *str, ...);

#endif /* COMMON_H_ */
