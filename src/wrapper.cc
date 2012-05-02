
#include <stdio.h> //TODO
#include <fuse.h>

#include "wrapper.h"
#include "fs.h"

using namespace collectd_fs;

inline filesystem *get_fs_ptr()
{
	struct fuse_context *context = fuse_get_context();
	if (context != NULL)
		return (filesystem *) context->private_data;
	return NULL;
}

int wrapper_open(const char *path, struct fuse_file_info *file_info)
{
	filesystem *fs = get_fs_ptr();
	return fs->open(string(path), file_info);
}

int wrapper_read(const char *path, char *buf, size_t size, off_t offset,
				   struct fuse_file_info *file_info)
{
	filesystem *fs = get_fs_ptr();
	return fs->read(string(path), buf, size, offset, file_info);
}

int wrapper_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
					  off_t offset, struct fuse_file_info *file_info)
{
	filesystem *fs = get_fs_ptr();
	return fs->readdir(string(path), buf, filler, offset, file_info);
}

int wrapper_getattr(const char *path, struct stat *stbuf)
{
	filesystem *fs = get_fs_ptr();
	return fs->getattr(string(path), stbuf);
}

