#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

static char *sqfs_parse_unc_path(const char *arg);
static char *squashfuse_path();

// const char *PROGNAME = "d:\\Dev\\squashfuse\\win\\x64\\Debug\\squashfuse.exe";
const char *volumePrefix = "--VolumePrefix=%s";

#define SQUASHFS_ARGS                      \
    "-orellinks",                       \
    "-ofstypename=squashfs",               \
    "-oUserKnownHostsFile=/dev/null",   \
    "-oStrictHostKeyChecking=no"

int main(int argc, char **argv)
{
	if (argc == 3) {
		const char *unc_path = argv[1];
		const char *drive = argv[2];
		char *archivePath = sqfs_parse_unc_path(unc_path);
		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), volumePrefix, unc_path);
		char *PROGNAME = squashfuse_path();
		char FNAME[_MAX_FNAME];
		char EXT[_MAX_EXT];
		_splitpath_s(PROGNAME, NULL, 0, NULL, 0, FNAME, _MAX_FNAME, EXT, _MAX_EXT);
		char ProgName[_MAX_FNAME + _MAX_EXT + 1];
		strcpy_s(ProgName, sizeof(ProgName), FNAME);
		strcat_s(ProgName, sizeof(ProgName), EXT);
		const char *squashfs_args[256] = {
//			PROGNAME, buffer, "-f", "-U", unc_path, drive, (void *)0
			ProgName, buffer, "-f", archivePath, drive, (void *)0
		};

		int retVal = (int)_spawnv(_P_WAIT ,PROGNAME, squashfs_args);
		if (retVal == -1) {
			fprintf(stderr, "Error: _spawnv failed. errorno %d\n", errno);
		}
		free(PROGNAME);
		free(archivePath);
		return retVal;
	}
	return 1;
}

#ifdef _M_X64
#define WIN_SQUASHFS_KEY L"SOFTWARE\\WOW6432Node\\WinFsp\\Services\\squashfs"
#else
#define WIN_SQUASHFS_KEY L"SOFTWARE\\WinFsp\\Services\\squashfs"
#endif

static
char *squashfuse_path()
{
	HKEY hKey;
	LSTATUS status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, WIN_SQUASHFS_KEY, 0, KEY_READ, &hKey);
	if (ERROR_SUCCESS == status) {
		DWORD bufSize = _MAX_PATH;
		char *buffer = (char *)calloc(_MAX_PATH, 3);
		if (buffer) {
			status = RegQueryValueExA(hKey, "Executable", NULL, NULL, buffer, &bufSize);
			if (ERROR_SUCCESS == status) {
				char drive[_MAX_DRIVE];
				char path[_MAX_PATH];
					
				_splitpath_s(buffer, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, 0, NULL, 0);
				_makepath_s(buffer, _MAX_PATH, drive, path, "squashfuse", ".exe");
				return buffer;
			}
		}
	}
	return NULL;
}

static
char *sqfs_parse_unc_path(const char *arg)
{

	/* This only applies when we're building against the WinFsp version of FUSE */

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
			/* Set the image path to the unc_path with the \server\share\ prefix stripped off.*/
			size_t sLen = strlen(unc_path) + 1;
			char *ptr = malloc(sLen);
			*ptr = *(++unc_path);
			*(ptr + 1) = ':';
			strcpy_s(ptr + 2, sLen - 2, ++unc_path);
			return ptr; //  ++unc_path;
		}
	}
	return NULL;
}