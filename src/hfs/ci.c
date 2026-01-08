/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *  ci.c - Functions for accessing Open Firmware's Client Interface
 *
 *  Copyright (c) 1998-2000 Apple Computer, Inc.
 *
 *  DRI: Josh de Cesare
 */

#include "sl.h"
#include "../diskio.h"
#include "../bootmii_ppc.h"
#include "../string.h"
#include "../types.h"

// Device I/O

#define SECTOR_SIZE 512

static long long gBasePosition = 0;
static long long gSeekPosition = 0;
static BYTE sector_buffer[SECTOR_SIZE] __attribute__((aligned(32)));

void SetBasePosition(long long basePosition)
{
  gBasePosition = basePosition;
}

// Seek takes an iHandle, and a 64 bit position
// and moves to that address in file.
// returns seeks result, or -1 if seek is not supported.
CICell Seek(CICell ihandle, long long position)
{
  (void)ihandle; // unused
  
  gSeekPosition = position;
  
  return 0;
}

// Read takes an iHandle, an address and a length and return the actual
// Length read.  Returns -1 if the operaction failed.
CICell Read(CICell ihandle, long addr, long length)
{
  (void)ihandle; // unused
  
  long long byte_offset = (gBasePosition + gSeekPosition) % SECTOR_SIZE;
  long long sector = (gBasePosition + gSeekPosition) / SECTOR_SIZE;
  BYTE *dest = (BYTE *)addr;
  long remaining = length;
  
  if (byte_offset > 0) {
    if (disk_read(0, sector_buffer, sector, 1) != RES_OK) {
      return -1;
    }
    
    long copy_len = SECTOR_SIZE - byte_offset;
    if (copy_len > remaining) {
      copy_len = remaining;
    }
    
    memcpy(dest, sector_buffer + byte_offset, copy_len);
    dest += copy_len;
    remaining -= copy_len;
    sector++;
  }
  
  if (remaining >= SECTOR_SIZE) {
    u32 full_sectors = remaining / SECTOR_SIZE;
    if (disk_read(0, dest, sector, full_sectors) != RES_OK) {
      return -1;
    }
    dest += full_sectors * SECTOR_SIZE;
    remaining -= full_sectors * SECTOR_SIZE;
    sector += full_sectors;
  }
  
  if (remaining > 0) {
    if (disk_read(0, sector_buffer, sector, 1) != RES_OK) {
      return -1;
    }
    memcpy(dest, sector_buffer, remaining);
  }

  return length;
}
