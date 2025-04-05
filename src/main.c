/*
        BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
        Requires mini.

Copyright (C) 2008, 2009        Haxx Enterprises <bushing@gmail.com>
Copyright (C) 2009              Andre Heider "dhewg" <dhewg@wiibrew.org>
Copyright (C) 2008, 2009        Hector Martin "marcan" <marcan@marcansoft.com>
Copyright (C) 2008, 2009        Sven Peter <svenpeter@gmail.com>
Copyright (C) 2009              John Kelley <wiidev@kelley.ca>
Copyright (C) 2025              Bryan Keller <kellerbryan19@gmail.com>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include "boot_args.h"
#include "bootmii_ppc.h"
#include "device_tree.h"
#include "string.h"
#include "ipc.h"
#include "macho_loader.h"
#include "mini_ipc.h"
#include "nandfs.h"
#include "fat.h"
#include "malloc.h"
#include "diskio.h"
#include "printf.h"
#include "video_low.h"
#include "input.h"
#include "console.h"
#include "irq.h"
#include "usb/core/core.h"
#include "usb/drivers/class/hid.h"
#include "sha1.h"
#include "hollywood.h"

static char ascii(char s) {
	if(s < 0x20) return '.';
	if(s > 0x7E) return '.';
	return s;
}

void hexdump(void *d, int len) {
	u8 *data;
	int i, off;
	data = (u8*)d;
	for (off=0; off<len; off += 16) {

		// Print address
		void *addr = (void *)((u32)data + off);
		printf("0x%08x  ", addr);

		// Print hex interpretation
		for (i=0; i<16; i++) {
            if ((i+off)>=len) {
                printf("   ");
            } else {
                printf("%02x ", data[off+i]);
            }
        }

		printf(" ");

		// Print ASCII interpretation
		for(i=0; i<16; i++) {
			if((i+off)>=len) {
				printf(" ");
			} else {
				printf("%c", ascii(data[off+i]));
			}
		}
		printf("\n");
	}
}

static void patch_memory_trap(u32 *offset) {
	offset[0] = 0x7FE00008; // trap
	sync_before_exec(offset, 1);
}

static void patch_memory_nop(u32 *offset) {
	offset[0] = 0x60000000; // ori, 0, 0, 0
	sync_before_exec(offset, 1);
}

static boot_args_t* set_up_boot_args() {
	boot_args_t *boot_args = (boot_args_t*)bootArgsAddress;

	boot_args->Revision = 1;

	boot_args->Version = 1;

	sprintf(boot_args->CommandLine, "-v\0");

	for (int i = 0; i < 26; i++) {
		boot_args->PhysicalDRAM[i].base = 0;
		boot_args->PhysicalDRAM[i].size = 0;
	}

	// MEM1 24MB @0x00000000
	boot_args->PhysicalDRAM[0].base = 0x00000000;
	boot_args->PhysicalDRAM[0].size = 24 * 1024 * 1024;

	// MEM2 64MB @0x10000000
	boot_args->PhysicalDRAM[1].base = 0x10000000;
	boot_args->PhysicalDRAM[1].size = 64 * 1024 * 1024;

	boot_args->Video.v_baseAddr = 0;
	boot_args->Video.v_display = 0;
	boot_args->Video.v_rowBytes = 0;
	boot_args->Video.v_width = 0;
	boot_args->Video.v_height = 0;
	boot_args->Video.v_depth = 0;

	boot_args->machineType = 0;

	build_device_tree();
	boot_args->deviceTreeP = (u8*)device_tree_start;
	boot_args->deviceTreeLength = device_tree_end - device_tree_start;

	boot_args->topOfKernelData = (u32)device_tree_end;

	return boot_args;
}

int main(void) {
	// Cannot gecko_init without delay
	udelay(1000000);
	gecko_init();

	printf("\nwiiMac\n");
	printf("(c) 2025 Bryan Keller - @blk19_\n\n");

	exception_init();

	irq_initialize();
	irq_bw_enable(BW_PI_IRQ_RESET);
	irq_bw_enable(BW_PI_IRQ_HW); //hollywood pic

	ipc_initialize();
	ipc_slowping();

	// int vmode = -1;
	// init_fb(vmode);
	// VIDEO_Init(vmode);
	// VIDEO_SetFrameBuffer(get_xfb());
	// VISetupEncoder();

	int ret;

	ret = load_mach_kernel("/mk");
	if (ret != 0) {
		return -1;
	}

	exception_init();

	patch_memory_trap((void*)0x000a0648);
	// patch_memory_trap((void*)0x0008a9f4);

	boot_args_t *boot_args = set_up_boot_args();

	print_device_tree(boot_args->deviceTreeP);

	ret = start_mach_kernel(boot_args);
	if (ret != 0) {
		return -1;
	}

    return 0;
}

