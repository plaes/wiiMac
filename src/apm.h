//
// Created by Bryan Keller on 1/7/26.
//

#ifndef APM_H
#define APM_H

#include "types.h"

typedef struct APM_Entry {
  char signature[2];      // 0x00
  u8 _reserved[2];        // 0x02
  u32 numPartitions;      // 0x04
  u32 startingSector;     // 0x08
  u32 _size;               // 0x0c
  char _name[32];         // 0x10
  char type[32];          // 0x30
  u8 _unused[432];        // 0x50
} apm_entry_t __attribute__((aligned(32)));

apm_entry_t apm_found_partitions[64];
int apm_found_partitions_count = 0;

int apm_find_partitions();

#endif //APM_H
