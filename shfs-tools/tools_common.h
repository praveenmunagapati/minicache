#ifndef _TOOLS_COMMON_
#define _TOOLS_COMMON_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "shfs_defs.h"

/*
 * Print helpers
 */
extern unsigned int verbosity;
extern int force;

#define eprintf(...)		fprintf(stderr, __VA_ARGS__)
#define fatal()			eprintf("%s\n", strerror(errno))
#define dief(...)		do { eprintf(__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#define die()			do { fatal(); exit(EXIT_FAILURE); } while(0)
#define dprintf(LEVEL, ...)	do { if (verbosity >= (LEVEL)) fprintf(stderr, __VA_ARGS__); } while(0)
#define printvar(VAR, FMT)	do { if (verbosity >= (D_MAX)) fprintf(stderr, #VAR ": "#FMT"\n", (VAR)); } while(0)

#define D_L0		1
#define D_L1		2
#define D_MAX		D_L1


/*
 * Argument parsing helper
 */
static inline int parse_args_setval_str(char** out, const char* buf)
{
	if (*out)
		free(*out);
	*out = strdup(buf);
	if (!*out) {
		*out = NULL;
		return -ENOMEM;
	}

	return 0;
}

static inline int parse_args_setval_int(int* out, const char* buf)
{
	if (sscanf(optarg, "%d", out) != 1)
		return -EINVAL;
	return 0;
}

/*
 * Disk I/O
 */
struct disk {
	int fd;
	char *path;
	uint64_t size;
	uint32_t blksize;
};

struct disk *open_disk(const char *path, int mode);
void close_disk(struct disk *d);

/*
 * Misc
 */
void print_shfs_hdr_summary(struct shfs_hdr_common *hdr_common,
                            struct shfs_hdr_config *hdr_config);
chk_t metadata_size(struct shfs_hdr_common *hdr_common,
                    struct shfs_hdr_config *hdr_config);
chk_t avail_space(struct shfs_hdr_common *hdr_common,
                  struct shfs_hdr_config *hdr_config);
static inline void hash_unparse(hash512_t h, uint8_t hlen, char *out)
{
	uint8_t i;

	for (i = 0; i < hlen; i++)
		snprintf(out + (2*i), 3, "%02x", h.u8[i]);
}

#endif /* _TOOLS_COMMON_ */
