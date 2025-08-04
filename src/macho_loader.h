//
// Created by Bryan Keller on 2/20/25.
//

#ifndef MACHO_LOADER_H
#define MACHO_LOADER_H

#define kernel_file_load_address (0x00c00000) // @ 12 MB
#define kernel_symtab_start (0x00500000) // @ 5 MB

u32 kernel_entry_point;

u32 kernel_header_start;
u32 kernel_header_size;

u32 kernel_text_start;
u32 kernel_text_size;

u32 kernel_data_start;
u32 kernel_data_size;

u32 kernel_symtab_size;

int load_mach_kernel(const char* kernel_path);

#endif //MACHO_LOADER_H
