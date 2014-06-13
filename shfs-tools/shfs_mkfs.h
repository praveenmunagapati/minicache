/*
 *
 */
#ifndef _SHFS_MKFS_
#define _SHFS_MKFS_

#include "tools_common.h"

#define STR_VERSION "Simon's HashFS Tools: MakeFS v1.02"

struct args {
	char **devpath;
	uint8_t nb_devs;

	uint8_t  encoding;
	char     volname[17]; /* null-terminated */
	uint32_t stripesize;

	int fullerase;
	int combined_striping;

	uint8_t  allocator;
	uint8_t  hashfunc;
	uint32_t hashlen;
	uint32_t bucket_count;
	uint32_t entries_per_bucket;
};

#endif /* _SHFS_MKFS_ */
