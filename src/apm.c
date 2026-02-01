//
// Created by Bryan Keller on 1/7/26.
//

#include "apm.h"
#include "diskio.h"
#include "string.h"

int find_partitions() {
  found_partitions_count = 0;
  
  apm_entry_t partition_table;
  if (disk_read(0, (BYTE *)&partition_table, 1, 1) != 0) {
    return -1;
  }
  
  if (strncmp(partition_table.signature, "PM", 2) != 0) {
    return -1;
  }
  
  // start at sector 2 since sector 1 is partition table
  for (u32 i = 0; i < partition_table.numPartitions; i++) {
    BYTE *found_partition = (BYTE *)&found_partitions[i];
    int sector = i + 1;
    if (disk_read(0, found_partition, sector, 1) != 0) {
      return -1;
    }
  }
  
  found_partitions_count = partition_table.numPartitions;

  return 0;
}
