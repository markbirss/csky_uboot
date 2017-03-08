/******************************************************************************
 * @file     datatype.c
 * @brief    The Head File for the csky datatype
 * @version  V1.0
 * @date     23. Dec 2016
 ******************************************************************************/
/* ---------------------------------------------------------------------------
 * Copyright (C) 2016 CSKY Limited. All rights reserved.
 *
 * Redistribution and use of this software in source and binary forms,
 * with or without modification, are permitted provided that the following
 * conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of CSKY Ltd. nor the names of CSKY's contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission of CSKY Ltd.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * -------------------------------------------------------------------------- */

#ifndef	__DATATYPE_H__
#define	__DATATYPE_H__
#include <linux/types.h>
////////////////////////////////////////////////////////////////////////////////////////
//
//

#ifndef NULL
#define	NULL  0x00
#endif

#ifndef TRUE
#define TRUE  0x01
#endif
#ifndef FALSE
#define FALSE 0x00
#endif

#ifndef true
#define true  0x01
#endif
#ifndef false
#define false 0x00
#endif

#ifndef SUCCESS
#define SUCCESS  0
#endif
#ifndef FAILURE
#define FAILURE  -1
#endif

#define	STATUS_ERR	1
#define	STATUS_OK	0

#define WREG volatile       uint32_t
#define SREG volatile       uint16_t
#define CREG volatile       uint8_t


typedef	unsigned char       CK_UINT8;
typedef unsigned short      CK_UINT16;
typedef unsigned int        CK_UINT32;
typedef	signed char         CK_INT8;
typedef signed short        CK_INT16;
typedef signed int          CK_INT32;
/* typedef signed long long    CK_INT64; */
typedef signed long long    CK_INT64;
typedef unsigned long long  CK_UINT64;
typedef unsigned int        BOOL;
//typedef CK_UINT8			bool;
#ifndef BYTE
typedef	unsigned char	    BYTE;
#endif
#ifndef WORD
typedef unsigned short	    WORD;
#endif
#ifndef DWORD
typedef unsigned int 	    DWORD;
#endif

#define CK_REG  volatile CK_UINT32
#define CK_SREG volatile CK_UINT16
#define CK_CREG volatile CK_UINT8

typedef struct
{
	CK_UINT16 year;
	CK_UINT8  month;
	CK_UINT8  day;
	CK_UINT8  weekday;
	CK_UINT8  hour;
	CK_UINT8  min;
	CK_UINT8  sec;
}__attribute__((packed)) RTCTIME, *PRTCTIME;


#if defined(DEBUG)
#define Debug     printf
#else
#define Debug
#endif

#define  IN
#define  OUT
#define INOUT

///////////////////////////////////////////////////////////////////////////////////////
#endif  // __DATATYPE_H__
///////////////////////////////////////////////////////////////////////////////////////
