/*
 * Platform_Types.h
 *
 *  Created on: 12 Feb., 2017
 *      Author: Hari
 */

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

/* File to include the basic data types for the Project */

#include "stdint.h" /* include basic integer handling header file */

/* Basic data types */
#ifdef __cplusplus
typedef bool BOOL; /* 1 bit */
#else
typedef unsigned char BOOL; /* 1 bit */
#endif

typedef unsigned char UBYTE; /* 8 bits */
typedef signed char SBYTE;
typedef unsigned short int UWORD; /* 16 bits */
typedef signed short int SWORD;
typedef unsigned long int ULONG; /* 32 bits */
typedef signed long int SLONG;

/* Short hand data types */
typedef BOOL B;
typedef UBYTE UB;
typedef SBYTE SB;
typedef UWORD UW;
typedef SWORD SW;
typedef ULONG UL;
typedef SLONG SL;

/* Basic Macros */
#ifdef __cplusplus
#define TRUE   true
#define FALSE  false
#else
#define TRUE   1
#define FALSE  0
#endif

#ifndef NULL
#define NULL ((void *)0)
#define NULL_PTR  ((void*)0)
#endif

#endif /* API_PLATFORM_TYPES_H_ */
