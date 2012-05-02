
#include <iostream>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fuse.h>

#include "fs.h"
#include "wrapper.h"

using namespace collectd_fs;
using std::cerr;
using std::cout;
using std::endl;

enum {
     KEY_HELP,
     KEY_VERSION,
	 KEY_DEBUG
};

struct collectd_fs_config
{
	int debug;
	char *address;
};

#define COLLECTD_FS_OPT(t, p, v) { t, offsetof(struct collectd_fs_config, p), v }

static struct fuse_opt collectd_fs_opts[] = {
	COLLECTD_FS_OPT("collectd-address=%s", address, 0),
	FUSE_OPT_KEY("-d",             KEY_DEBUG),
	FUSE_OPT_KEY("-V",             KEY_VERSION),
	FUSE_OPT_KEY("--version",      KEY_VERSION),
	FUSE_OPT_KEY("-h",             KEY_HELP),
	FUSE_OPT_KEY("--help",         KEY_HELP),
	FUSE_OPT_END
};

static struct fuse_operations collectd_fs_opers; /* Initialized below */


static int collectd_fs_opt_proc(void *data, const char *arg, 
								int key, struct fuse_args *outargs)
{
     switch (key) {

	 case KEY_DEBUG:
	 {
		 struct collectd_fs_config *conf = (struct collectd_fs_config*) data;
		 conf->debug = 1;
		 fuse_opt_add_arg(outargs, "-d");
		 break;
	 }

     case KEY_HELP:
	 {
		 cerr <<
		         "usage: " <<
				 outargs->argv[0] <<
				 "mountpoint [options]\n"
				 "\n"
				 "general options:\n"
				 "    -o opt,[opt...]  mount options\n"
				 "    -h   --help      print help\n"
				 "    -V   --version   print version\n"
				 "\n"
				 "collectdfs options:\n"
                     "    -o log=STRING\n"
				     "    -o collectd-address=server address"
			  << endl;

		 fuse_opt_add_arg(outargs, "-ho");
		 fuse_main(outargs->argc, outargs->argv, &collectd_fs_opers, NULL);
		 exit(1);
	 }

     case KEY_VERSION:
	 {
		 cerr << "collectdfs version " << PACKAGE_VERSION << endl;
		 fuse_opt_add_arg(outargs, "--version");
		 fuse_main(outargs->argc, outargs->argv, &collectd_fs_opers, NULL);
		 exit(0);
	 }
     }
     return 1;

}

int main(int argc, char **argv)
{
	memset(&collectd_fs_opers, 0, sizeof(collectd_fs_opers));

	collectd_fs_opers.readdir = wrapper_readdir;
	collectd_fs_opers.getattr = wrapper_getattr;
	collectd_fs_opers.open    = wrapper_open;
	collectd_fs_opers.read    = wrapper_read;

	filesystem *fs = new filesystem();

	if (fs == NULL) {

		cerr << "Failed to allocate memory for filesystem object" << endl;
		return 1;
	}

	struct collectd_fs_config config;

	memset(&config, 0, sizeof(config));

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	fuse_opt_parse(&args, &config, collectd_fs_opts, collectd_fs_opt_proc);

	if (config.debug) {
		cout << "[ collectf-fs ]: Enabling debug" << endl;
		fs->enable_debug();
	}

	collectd_client *client = new collectd_client();

	if (config.address != NULL) {
		if (client->connect(config.address)) {
			cerr << "Couldn't connect to collectd at address "
				 << config.address
				 << endl;
			return 1;
		}

		fs->init(client);

		return fuse_main(args.argc, args.argv, &collectd_fs_opers, fs);
	}
	else
	{
		cerr << "Address of collectd was not specified" << endl;

	}




}

