/*
 * Copyright (c) 2019 Daniel R. Slater <dslater38@gmail.com>
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

// forward declare IUnknown or we get a compile error out of <combaseapi.h>
struct IUnknown;

#include <windows.h>
#include <winfsp/winfsp.h>
#include <signal.h>
#include <process.h>

extern "C"
{
	/* delay-load winfsp dynamic link library. */
	/* return 0 on success. */
	int sqfs_load_winfsp()
	{

		if (!NT_SUCCESS(FspLoad(0)))
			return (ERROR_DELAY_LOAD_FAILED);
		return 0;

	}
	int glb__WinFspLoaded = sqfs_load_winfsp();
#if 0
	struct Args {
		int argc;
		char **argv;
	};

	static unsigned thread_proc(void *p)
	{
		extern int t_main(int argc, char **argv);
		if (p) {
			struct Args *pArgs = (struct Args *)p;
			return t_main(pArgs->argc, pArgs->argv);
		}
	}

	NTSTATUS SvcStart(FSP_SERVICE *Service, ULONG wargc, PWSTR *wargv)
	{
		FspServiceAllowConsoleMode(Service);
		struct Args *tArgs = (struct Args *)Service->UserContext;
		if (tArgs) {
			// _beginthreadex(NULL, 0, thread_proc, tArgs, 0, NULL);
			thread_proc(tArgs);

			return STATUS_SUCCESS;
		}
		else {
			return STATUS_ACCESS_DENIED;
		}
	}

	NTSTATUS SvcStop(FSP_SERVICE *Service)
	{
		raise(SIGTERM);
		return STATUS_SUCCESS;
	}

	int wmain(int wargc, wchar_t **wargv)
	{
		int argc = (int)wargc;
		size_t len = 0;

		for (int i = 0; i < argc; ++i) {
			len += wcslen(wargv[i]) + 1;
		}

		size_t alloc_len = len + argc * sizeof(char *);
		char **argv = (char **)malloc(alloc_len);
		char *data = (char *)(argv + argc);
		for (int i = 0; i < argc; ++i) {
			argv[i] = data;
			WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, data, alloc_len, NULL, NULL);
			data += strlen(argv[i]) + 1;
			alloc_len -= strlen(argv[i]) + 1;
		}

		struct Args *tArgs = (struct Args *)malloc(sizeof(struct Args));
		tArgs->argc = argc;
		tArgs->argv = argv;

		return FspServiceRunEx((PWSTR)L"squashfs", SvcStart, SvcStop, 0, tArgs);
	}

#endif
}
