
#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <list>
#include <string>

using std::list;
using std::string;

namespace collectd_fs
{

	class entry
	{
	public:
		entry(string stat_name);
		~entry();
		
		entry *add_sub_entry(string token);
	
		bool operator <(const entry &rhs) {
			return token() < rhs.token();
		}
		
		string token() const { return m_token; }
		bool is_leaf() const { return (m_sub_entries.size() == 0); } 
		list<entry *>& sub_entries() { return m_sub_entries; }

		string complete_stat_name() const { return m_complete_stat_name; }
		void complete_stat_name(string str) { m_complete_stat_name = str; }

	private:
		entry();

	private:
		list<entry *> m_sub_entries;
		string        m_token;
		string        m_complete_stat_name;
		time_t        m_last_update_time;
	};

	typedef list<entry *>::iterator entry_iterator;
};

#endif /* _ENTRY_H_ */
