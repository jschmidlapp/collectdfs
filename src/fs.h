
#ifndef _FS_H_
#define _FS_H_

#include <fuse.h>
#include <string>

#include "client.h"
#include "common.h"
#include "stat_tree.h"

using std::string;

namespace collectd_fs
{
	class filesystem
	{
	public:
		filesystem();
		~filesystem();

		int init(collectd_client *client);
		

		void enable_debug() { m_log_level = LOG_LEVEL_DEBUG; }

		int open(string path, struct fuse_file_info *file_info);

		int read(string path, char *buf, size_t size, off_t offset, 
				 struct fuse_file_info *file_info);

		int readdir(string path, void *buf, fuse_fill_dir_t filler,
					off_t offset, struct fuse_file_info *file_info);

		int getattr(string path, struct stat *stbuf);

	private:
		int               m_log_level;
		stat_tree        *m_stat_tree;
		collectd_client  *m_client;

	private:
		uint32_t get_num_subdirs(entry *file_entry);

	};
};

#endif /* _FS_H_ */
