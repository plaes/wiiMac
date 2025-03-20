//
// Created by Bryan Keller on 2/20/25.
//

#ifndef MACHO_LOADER_H
#define MACHO_LOADER_H

#define kernelImageAddress (0x00004000)
#define kernelFileLoadAddress (0x00C00000) // @ 12 MB

int load_mach_kernel(const char* kernel_path);
int start_mach_kernel();

#endif //MACHO_LOADER_H
