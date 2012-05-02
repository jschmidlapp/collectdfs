
#include <iostream> // todo

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "fs.h"
#include "stat_tree.h"

using collectd_fs::filesystem;
using std::string;

filesystem::filesystem() :
	m_log_level(LOG_LEVEL_ERROR),
	m_stat_tree(NULL)
{}

filesystem::~filesystem() 
{
	if (m_stat_tree == NULL)
		delete m_stat_tree;
}

int filesystem::init(collectd_client *client)
{
	m_stat_tree = new stat_tree();

	if (m_stat_tree == NULL)
		return -1;

	m_stat_tree->init(client);

	m_client = client;

	return 0;
}

int filesystem::open(string path, struct fuse_file_info *file_info)
{
	DEBUG("open - %s\n", path.c_str());

	entry *file_entry = m_stat_tree->get_entry(path);

	if (file_entry) {
		file_info->fh = (uint64_t) file_entry;
		return 0;
	}
	else
		return -ENOENT;

	return 0;
}

int filesystem::read(string path, char *buf, size_t size, off_t offset, 
					 struct fuse_file_info *file_info)
{
	DEBUG("read - %s\n", path.c_str());
	
	entry *file_entry = (entry *) file_info->fh;

	string stat_name  = file_entry->complete_stat_name();
	string stat_value = m_client->get_stat(stat_name);

	size_t len = stat_value.size();

	/* Does offset always have to be positive? */
	if ((size_t) offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, stat_value.c_str() + offset, size);
	}
	else 
		size = 0; /* Offset is past end of file */

	return size;
}


int filesystem::readdir(string path, void *buf, fuse_fill_dir_t filler,
						off_t offset, struct fuse_file_info *file_info)
{
	DEBUG("readdir - %s\n", path.c_str());

	m_stat_tree->refresh();

	entry *file_entry = m_stat_tree->get_entry(path);

	if (file_entry == NULL)
		return -ENOENT;

	struct stat st;

	memset(&st, 0, sizeof(struct stat));
	st.st_mode = S_IFDIR;
	filler(buf, ".", &st, 0);
	filler(buf, "..", &st, 0);
	
	entry_iterator iter;

	foreach(iter, file_entry->sub_entries()) {
		if ((*iter)->is_leaf())
			st.st_mode = S_IFREG;
		else
			st.st_mode = S_IFDIR;

		filler(buf, (*iter)->token().c_str(), &st, 0);
	}

	return 0;
}

int filesystem::getattr(string path, struct stat *stbuf)
{

	DEBUG("getattr - %s\n", path.c_str());

	memset(stbuf, 0, sizeof(*stbuf));

	m_stat_tree->refresh();

	entry *file_entry = m_stat_tree->get_entry(path);

	if (file_entry == NULL)
		return -ENOENT;

	if (file_entry->is_leaf()) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;

		stbuf->st_size = file_entry->complete_stat_name().size();
	}
	else {
		stbuf->st_mode = S_IFDIR | 0555;
		stbuf->st_nlink = 2 + get_num_subdirs(file_entry);
		stbuf->st_size = file_entry->token().length();
	}

	return 0;
}

uint32_t filesystem::get_num_subdirs(entry *file_entry)
{
	assert(file_entry);

	uint32_t num_subdirs = 0;
	entry_iterator iter;

	foreach(iter, file_entry->sub_entries()) {
		if (((*iter)->sub_entries()).size() > 0)
			num_subdirs++;
	}

	return num_subdirs;
}
