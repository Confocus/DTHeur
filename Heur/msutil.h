/*
** Copyright (C) 2009-2014 Mischa Sandberg <mischasan@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License Version 3 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU Lesser General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

// msutil: convenience functions.
// MEM: manipulating byte-blocks and slices thereaof.
//  "MEMBUF" is a <len,ptr> descriptor of a malloc'd block of memory.
//          The owner of the MEMBUF variable also "owns" the memory.
//  "MEMREF" is a <len,ptr> descriptor of memory allocated elsewhere.
//          A MEMREF can describe a substring of a MEMBUF value.
//	membuf(size)                    Create membuf; membuf(0) => NILBUF
//	buffree(buf)
//	memref(char const*mem, int len) Create memref from (ptr,len)
//	nilbuf(buf)                     Test if buf is a NILBUF
//	nilref(ref)                     Test if ref is a NILREF
//	bufref(buf)                     Create MEMREF from a MEMBUF; NILBUF => NILREF
//
//  chomp(buf)              - Remove trailing >>WHITESPACE<< from a MEMBUF.
//                              chomp(NILBUF) = NILBUF.
//  exit_proc(fmt, ...)           - Print message to stderr and exit(1).
//                              If fmt begins with ":", die prepends program name.
//                              If fmt ends in ":", die appends strerror(errno).
//  errname[nerrnames]      - (string) names for errno values. More succinct than strerror(). 
//  getprogname()           - Returns pointer to the program name (basename).
//  refsplit(s,sep,&cnt)    - split text. Replaces every (sep) with \0 in (text).
//                              Returns malloc'd vector of [cnt] MEMREF's.
//  slurp(fname)            - create membuf from a file. fname "-" or NULL reads stdin.
//  tick()                  - High-precision timer returns secs as a (double).
//  usage()                 - generic print program name, usage; then exit(2)
//--------------|---------------------------------------------

#ifndef MSUTIL_H
#define MSUTIL_H

#include <stdint.h>
#include "stdafx.h"
// Defeat gcc 4.4 cdefs.h defining __wur i.e. __attribute((unused-result))
// for system calls where you just don't care (e.g. vasprintf...)
// This must follow "#include <stdint.h>" and precede everything else.
#undef	__wur
#define __wur

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_MSC_VER)
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
#endif

typedef struct { 
	char *ptr; 
	size_t len; 
}MEM_BUF;

//typedef struct { 
//	char const *ptr; 
//	size_t len; 
//}MEMREF;

#define	NULL_MEM_BUF		{NULL, 0}
#define	NILREF		(MEMREF){NULL, 0}

#define ASSERT_MEMBUF_NULL(buff)	(NULL == buff.ptr || 0 == buff.len)

void    buffree(MEM_BUF buf);
MEMREF  bufref(MEM_BUF const buf); 
MEM_BUF  chomp(MEM_BUF buf);
void    exit_proc(char const *fmt, ...);
acism_bool  membuf(int size, MEM_BUF** ppBuf);
MEMREF  memref(char const *mem, int len);
int     nilbuf(MEM_BUF buf);
int     nilref(MEMREF const ref);
//MEMREF* SigSplit(char *text, char sep, int *pnrefs);
//acism_bool Load_Sig_Db(const char *filename, MEM_BUF** ppBuf);
//acism_bool acism_load_sign(const char *filename, MEMREF** ppRef, int* pSigNum);
double  tick(void);
void    usage(char const *);

#if defined(__linux__)
char const *getprogname(void);  // BSD-equivalent
#endif

extern int  const nerrnames;
extern char const *errname[];

#endif//MSUTIL_H
