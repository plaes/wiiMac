/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (C) 2009		Andre Heider "dhewg" <dhewg@wiibrew.org>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include "fat.h"

static FATFS fatfs;
static u32 partition_start = 0;

u32 fat_mount(u32 partition_start_sector) {
	DSTATUS stat;

	f_mount(0, NULL);

	stat = disk_initialize(0);

	if (stat & STA_NODISK)
		return -1;

	if (stat & STA_NOINIT)
		return -2;
  
  if (f_mount(0, &fatfs) != FR_OK)
    return -3;

  partition_start = partition_start_sector;
	return 0;
}

u32 fat_umount(void) {
	f_mount(0, NULL);

	return 0;
}

u32 fat_clust2sect(u32 clust) {
	return clust2sect(&fatfs, clust);
}

u32 fat_get_partition_start_sector() {
  return partition_start;
}
