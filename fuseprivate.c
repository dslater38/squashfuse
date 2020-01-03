/*
 * Copyright (c) 2014 Dave Vasilevsky <dave@vasilevsky.ca>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fuseprivate.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nonstd.h"


sqfs_err sqfs_stat(sqfs *fs, sqfs_inode *inode, struct fuse_stat *st) {
	sqfs_err err = SQFS_OK;
	uid_t id;
	
	memset(st, 0, sizeof(*st));
	st->st_mode = inode->base.mode;
	st->st_nlink = inode->nlink;
	st->st_mtim.tv_sec = st->st_ctim.tv_sec = st->st_atim.tv_sec = inode->base.mtime;
	
	if (S_ISREG(st->st_mode)) {
		/* FIXME: do symlinks, dirs, etc have a size? */
		st->st_size = inode->xtra.reg.file_size;
		st->st_blocks = st->st_size / 512;
	} else if (S_ISBLK(st->st_mode) || S_ISCHR(st->st_mode)) {
		st->st_rdev = sqfs_makedev(inode->xtra.dev.major,
			inode->xtra.dev.minor);
	} else if (S_ISLNK(st->st_mode)) {
		st->st_size = inode->xtra.symlink_size;
	}

	st->st_blksize = fs->sb.block_size; /* seriously? */

	err = sqfs_id_get(fs, inode->base.uid, &id);
	if (err)
		return err;
	st->st_uid = id;
	err = sqfs_id_get(fs, inode->base.guid, &id);
	st->st_gid = id;
	if (err)
		return err;
	
	return SQFS_OK;
}

int sqfs_listxattr(sqfs *fs, sqfs_inode *inode, char *buf, size_t *size) {
	sqfs_xattr x;
	size_t count = 0;
	
	if (sqfs_xattr_open(fs, inode, &x))
		return -EIO;
	
	while (x.remain) {
		size_t n;
		if (sqfs_xattr_read(&x))
			 return EIO;
		n = sqfs_xattr_name_size(&x);
		count += n + 1;
		
		if (buf) {
			if (count > *size)
				return ERANGE;
			if (sqfs_xattr_name(&x, buf, true))
				return EIO;
			buf += n;
			*buf++ = '\0';
		}
	}
	*size = count;
	return 0;
}

void sqfs_usage(char *progname, bool fuse_usage) {
	const char *prog = progname ? progname : PACKAGE_NAME;
	fprintf(stderr, "%s (c) 2012 Dave Vasilevsky\n\n", PACKAGE_STRING);
	fprintf(stderr, "Usage: %s [options] ARCHIVE MOUNTPOINT\n", prog);

#ifdef FUSE_WINFSP_FUSE_H_INCLUDED
	/* This only applies when we're building against the WinFsp version of FUSE */
	fprintf(stderr, "\n%s options:\n", prog);
	fprintf(stderr, "-U\t indicates ARCHIVE is a unc path name of the form \\[WINFSP_SERVICE]\\ARCHIVE_PATH\n");
	fprintf(stderr, "\t v%s\n", "where [WINFSP_SERVICE] is the name of the WinFsp service key that enables the WinFsp Network provider.");
	fprintf(stderr, "\t For info on the WinFsp network provider, see:\n\t%s\n", "https://github.com/billziss-gh/winfsp/blob/master/doc/WinFsp-Service-Architecture.asciidoc#winfsp-network-provider");
#endif

	if (fuse_usage) {
		struct fuse_args args = FUSE_ARGS_INIT(0, NULL);
		fuse_opt_add_arg(&args, ""); /* progname */
		fuse_opt_add_arg(&args, "-ho");
		fprintf(stderr, "\n");
		fuse_parse_cmdline(&args, NULL, NULL, NULL);
		sqfs_print_compression_info();
	}
	exit(-2);
}

#ifdef FUSE_WINFSP_FUSE_H_INCLUDED
/* This only applies when we're building against the WinFsp version of FUSE */
static
int sqfs_opt_add_volume_prefix_arg(const char *prefixStart, const char *prefixEnd, struct fuse_args *outargs) {
	const int slen = strlen(prefixStart);/* (int)(prefixEnd - prefixStart); */
	const char *prefix_format = "--VolumePrefix=%.*s";
	if (slen > 0)
	{
		char *buf = calloc(strlen(prefix_format) + slen + 5, 1);
		if (buf) {
			sprintf(buf, prefix_format, slen, prefixStart);
			fuse_opt_add_arg(outargs, buf);
			// fuse_opt_add_arg(outargs, "-ofstypename=squashfs");
			free(buf);
			return 1;
		}
	}

	return 0;
}
#endif

static
int sqfs_parse_unc_path(sqfs_opts *opts, const char *arg, struct fuse_args *outargs)
{
	int success = 0;
#ifdef FUSE_WINFSP_FUSE_H_INCLUDED
	/* This only applies when we're building against the WinFsp version of FUSE */
	if (opts->have_unc_path == 1) {
		const char *unc_path = arg;
		/* skip over the server and share names */
		const char *unc_prefix = unc_path;
		if (*unc_prefix == '\\') {
			int i = 0;
			for (i = 0; i < 2 && (*unc_path == '\\'); ++i) {
				unc_path = strchr(++unc_path, '\\');
			}

			if (i == 2 && unc_path != NULL) {
				/* Add the WinFsp VolumePrefix argument so we show up as a network drive */
				/* sqfs_opt_add_volume_prefix_arg(unc_prefix, unc_path, outargs); */
				/* Set the image path to the unc_path with the \server\share\ prefix stripped off.*/
				char *ptr = malloc(strlen(unc_path) + 1);
				*ptr = *(++unc_path);
				*(ptr+1) = ':';
				strcpy(ptr + 2, ++unc_path);
				opts->image = ptr; //  ++unc_path;
				success = 1;

			}
		}
	}
#endif 
	return success;
}

int sqfs_opt_proc(void *data, const char *arg, int key,
		struct fuse_args *outargs) {
	sqfs_opts *opts = (sqfs_opts*)data;
	if (key == FUSE_OPT_KEY_NONOPT) {
		if (opts->mountpoint) {
			return -1; /* Too many args */
		} else if (opts->image) {
			opts->mountpoint = 1;
			return 1;
		} else if (sqfs_parse_unc_path(opts, arg, outargs) == 0) {
			opts->image = arg;
		}
		return 0;
	} else if (key == FUSE_OPT_KEY_OPT) {
		if (strncmp(arg, "-h", 2) == 0 || strncmp(arg, "--h", 3) == 0) {
			sqfs_usage(opts->progname, true);
		}
		else if (strncmp(arg, "-U", 2) == 0 || strncmp(arg, "--U", 3) == 0) {
			opts->have_unc_path = 1;
		}
	}
	return 1; /* Keep */
}
