/******************************************************************************
 *
 *    nio_types.h
 *
 *    Copyright (C) 2005, 2007  Chris Nystrom
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU Lessor General Public License as published
 *    by the Free Software Foundation; either version 2.1 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but without any warranty whatsoever.
 *
 *    For more license information see:
 *
 *      http://www.gnu.org/copyleft/lessor.html
 *
 *    Contact Author at:
 *
 *    Chris Nystrom
 *
 *    E-Mail: cnystrom@newio.org
 *
 *    Soli Deo Gloria
 *
 */

#ifndef _TYPES_H
#define _TYPES_H

#ifndef PRIVATE
#define PRIVATE static
#endif				/*  */

#ifndef PUBLIC
#define PUBLIC
#endif				/*  */

typedef char int8;
typedef unsigned char uint8;
typedef signed char sint8;
typedef short int16;
typedef unsigned short uint16;
typedef signed short sint16;
typedef int int32;
typedef unsigned int uint32;
typedef signed int sint32;

#ifndef _BYTE_H
#define _BYTE_H
typedef unsigned char byte;
typedef unsigned char *byte_ptr;
#endif				// _BYTE_H

#ifndef _CHAR_PTR
#define _CHAR_PTR
typedef char *char_ptr;
#endif				/*  */

#define BYTE_SIZE   (1)		// Haha
#define INT8_SIZE   (1)
#define INT16_SIZE  (2)
#define INT32_SIZE  (4)
#define FLT_SIZE    (4)
#define DBL_SIZE    (8)
#define WCHAR_SIZE  (4)		// For unicode chars

#ifndef _BOOL_H
#define _BOOL_H

// Not compatible with C++...
/*
typedef enum { FALSE = 0, TRUE = 1
} bool;
*/

#ifndef FALSE
#define FALSE    (0)
#endif

#ifndef TRUE
#define TRUE    !(FALSE)
#endif

#endif				// _BOOL_H

#endif				// _TYPES_H
