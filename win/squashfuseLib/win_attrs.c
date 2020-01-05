#include <windows.h>
#include "../squashfuse.h"

unsigned short sqfs_inode_mode_to_stat_mode(uint32_t mode)
{
	unsigned short xmode = 0;
	if (S_ISDIR(mode)) {
		xmode |= _S_IFDIR;
	}
	if (S_ISCHR(mode)) {
		xmode |= _S_IFCHR;
	}
	if (S_ISFIFO(mode)) {
		xmode |= _S_IFIFO;
	}
	if (S_ISREG(mode)) {
		xmode |= _S_IFREG;
	}
	if (S_ISFIFO(mode)) {
		xmode |= _S_IFIFO;
	}
	// this bit is not defined for windows - just use the posix bit
	if (S_ISLNK(mode)) {
		xmode |= S_IFLNK;
	}
	// this bit is not defined for windows - just use the posix bit
	if (S_ISBLK(mode)) {
		xmode |= S_IFBLK;
	}
	// this bit is not defined for windows - just use the posix bit
	if (S_ISSOCK(mode)) {
		xmode |= S_IFSOCK;
	}

	if ((mode & (S_IRUSR | S_IRGRP | S_IROTH)) != 0) {
		xmode |= _S_IREAD;
	}

	if ((mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0) {
		xmode |= _S_IEXEC;
	}
	/* not going to check for write perms. This is q read-only file system, so no write perms */
	return xmode;
}
