
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <list>
#include <collectd/client.h>

using std::string;
using std::list;

namespace collectd_fs
{
	class collectd_client
	{
	public:
		collectd_client();
		int connect(string address);
		string get_stat(string stat_name);

		list<string> get_stat_list();

	private:
		::lcc_connection_t *m_handle;

		static const int MAX_STAT_STR_LEN = 64;
	};
}

#endif /* _CLIENT_H_ */
