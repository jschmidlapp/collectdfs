
#include <stdio.h> // todo
#include <iostream> // todo

#include <sstream>
#include <algorithm>
#include <ctime>

#include "stat_tree.h"
#include "common.h"

using collectd_fs::stat_tree;
using collectd_fs::entry;

stat_tree::stat_tree() :
	m_last_tree_update(0)
{}

stat_tree::~stat_tree()
{}

int stat_tree::init(collectd_client *client)
{
	m_root_entry = new entry("/");

	if (m_root_entry == NULL) 
		return -1;

	if (client == NULL)
		return -1;

	m_client = client;

	return 0;
}

int stat_tree::populate(list<string> stat_list)
{
	list<string>::iterator iter;

	foreach(iter, stat_list) {

		entry *curr_entry = m_root_entry;
		string str = *iter;

		string complete_stat_str = str;

		/* Convert slashes to spaces so we can parse with stringstream */
		std::replace(str.begin(), str.end(), '/', ' ');

		/* Replace the FIRST hypen with a space.  This will cause the initial
		 * stat names (e.g. cpu-0, cpu-1) to be split into seperate directories
		 * (cpu/0, cpu/1) but will keep hypenated names later on (e.g. df-root)
		 */
		size_t first_hyphen_loc = str.find("-");
		if (first_hyphen_loc != string::npos)
		{
			size_t first_space_loc = str.find(" ");
			if (first_space_loc != string::npos)
			{
				size_t second_space_loc = str.find(" ", first_space_loc+1);

				if ( (second_space_loc != string::npos) &&
					 (first_hyphen_loc < second_space_loc) )
				{
					str.replace(first_hyphen_loc,1," ");
				}
			}
		}

		std::cout << str << std::endl;
		std::stringstream ss(str);

		while (ss.good()) {
			string token;

			ss >> token;

			curr_entry = curr_entry->add_sub_entry(token);
		}

		curr_entry->complete_stat_name(complete_stat_str);

	}

	return 0;
}

void stat_tree::refresh()
{
	time_t current_time;

	time(&current_time);

	if (difftime(current_time, m_last_tree_update) > TREE_UPDATE_SECS) {

		delete m_root_entry;
		m_root_entry = new entry("/");

		if (m_root_entry != NULL)
		{
			populate(m_client->get_stat_list());
			m_last_tree_update = current_time;
		}
		else
		{
			//TODO: handle memory allocation error
		}
	}
}

entry *stat_tree::get_entry(const string path)
{
	string str = path;

	/* Convert slashes to spaces so we can parse with stringstream */
	std::replace(str.begin(), str.end(), '/', ' ');
	
	std::stringstream ss(str);

	entry *curr_entry = m_root_entry;

	if (curr_entry->token() == path) {
		return curr_entry;
	}

	while (ss.good()) {
		string token;
		
		ss >> token;

		/* We only want a leaf - otherwise, we'll match prematurely if the stat string
		 * contains two identical tokens (which is permissible)
		 */
		if ( (curr_entry->token() == token) &&
			 (curr_entry->is_leaf()) )
		{
			return curr_entry;
		}

		entry_iterator iter;

		foreach(iter, curr_entry->sub_entries()) {
			if ((*iter)->token() == token) {
				curr_entry = (*iter);
				break;
			}
		}

	}

	/* 
	 * We didn't consume all of the tokens, so we couldn't find the
	 * full stat name.
	 */
	if (ss.good()) 
		return NULL;

	return curr_entry;
}

