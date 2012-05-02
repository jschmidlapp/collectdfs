
#include "common.h"
#include "entry.h"

using namespace collectd_fs;
using collectd_fs::entry;

entry::entry(string token) :
	m_token(token),
	m_last_update_time(0)
{}

entry::~entry()
{
	entry_iterator iter;
	
	foreach(iter, m_sub_entries) {
		if (*iter != NULL)
			delete *iter;
	}
}

entry *entry::add_sub_entry(string token)
{
	entry_iterator iter;

	foreach(iter, m_sub_entries) {
		if ((*iter)->m_token == token)
			return *iter;
	}

	entry *new_entry = new entry(token);

	m_sub_entries.push_back(new_entry);
	m_sub_entries.sort();

	return new_entry;
}
