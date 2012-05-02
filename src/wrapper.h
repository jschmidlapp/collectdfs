
#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include <fuse.h>

int   wrapper_open(const char *path, struct fuse_file_info *file_info);
int   wrapper_read(const char *path, char *buf, size_t size, off_t offset,
				   struct fuse_file_info *file_info);
int   wrapper_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
					  off_t offset, struct fuse_file_info *file_info);
int   wrapper_getattr(const char *path, struct stat *stbuf);

#endif /* _WRAPPER_H_ */
