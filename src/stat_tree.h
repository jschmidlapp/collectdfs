
#ifndef _STAT_TREE_H_
#define _STAT_TREE_H_

#include <list>
#include <string>

#include "entry.h"
#include "client.h"

using std::list;
using std::string;

namespace collectd_fs
{
	class stat_tree
	{
	public:
		stat_tree();
		~stat_tree();

		int init(collectd_client *client);
		void refresh();
		entry *get_entry(string path);

	private:
		entry *m_root_entry;
		collectd_client *m_client;
		list<string> m_stat_list;

	private:
		int populate(list<string> stat_list);

		time_t m_last_tree_update;

		static const int TREE_UPDATE_SECS = 5;
	};
};

#endif /* _STAT_TREE_H_ */
