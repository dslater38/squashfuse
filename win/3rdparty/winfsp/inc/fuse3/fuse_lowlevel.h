#ifndef FUSE_LOWLEVEL_H_INCLUDED
#define FUSE_LOWLEVEL_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

	/** The node ID of the root inode */
#define FUSE_ROOT_ID 1

/** Request pointer type */
	typedef struct fuse_req *fuse_req_t;


	inline void fuse_cmdline_help(void)
	{
		printf("    -h   --help            print help\n"
			"    -V   --version         print version\n"
			"    -d   -o debug          enable debug output (implies -f)\n"
			"    -f                     foreground operation\n"
			"    -s                     disable multi-threaded operation\n"
			"    -o clone_fd            use separate fuse device fd for each thread\n"
			"                           (may improve performance)\n"
			"    -o max_idle_threads    the maximum number of idle worker threads\n"
			"                           allowed (default: 10)\n");
	}

#ifdef __cplusplus
}
#endif

#endif // FUSE_LOWLEVEL_H_INCLUDED
