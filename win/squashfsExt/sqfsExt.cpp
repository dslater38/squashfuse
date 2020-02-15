// sqfsExt.cpp : Implementation of CsqfsExt

#include "pch.h"
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <winternl.h>
#pragma warning(push)
#pragma warning(disable:4005)           /* macro redefinition */
#include <ntstatus.h>
#pragma warning(pop)

#include "sqfsExt.h"
#include <atlconv.h>  // for ATL string conversion macros
#include <stdarg.h>
#include <stdio.h>
#include "resource.h"
#include <atlstr.h>
#include "MountSQFSArchive.h"

#define FSP_LIBPATH L"bin\\"
#if defined(_WIN64)
#define INSTALL_KEY L"Software\\WOW6432Node\\WinFsp"
#define FSP_LAUNCHERNAME _T("launchctl-x64.exe")
#else
#define INSTALL_KEY L"Software\\WinFsp"
#define FSP_LAUNCHERNAME _T("launchctl-x86.exe")
#endif

#ifdef _DEBUG
static void logError(const char *fmt, ...)
{
	FILE *fp = NULL;
	fopen_s(&fp, "C:\\Users\\dslat\\sqfLog.txt", "a+");
	if (fp)
	{
		va_list args;
		va_start(args, fmt);
		vfprintf(fp, fmt, args);
		va_end(args);
		fflush(fp);
		fclose(fp);
	}
}
#else
static inline void logError(const char *, ...)
{

}
#endif

STDMETHODIMP CsqfsExt::Initialize(
	LPCITEMIDLIST pidlFolder,
	LPDATAOBJECT pDataObj,
	HKEY hProgID)
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT,
					  -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	logError("CsqfsExt::Initialize() CALLED\n");

	// Look for CF_HDROP data in the data object. If there
	// is no such data, return an error back to Explorer.
	if (FAILED(pDataObj->GetData(&fmt, &stg)))
	{
		logError("pDataObj->GetData(&fmt, &stg) failed\n");
		return E_INVALIDARG;
	}

	// Get a pointer to the actual data.
	hDrop = (HDROP)GlobalLock(stg.hGlobal);

	// Make sure it worked.
	if (NULL == hDrop)
	{
		logError("GlobalLock(stg.hGlobal) failed\n");
		return E_INVALIDARG;
	}
	// Sanity check – make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	HRESULT hr = S_OK;

	if (0 == uNumFiles)
	{
		logError("DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0) failed\n");
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
	}

	// Get the name of the first file and store it in our
	// member variable m_szFile.
	if (0 == DragQueryFile(hDrop, 0, m_szFile, MAX_PATH))
	{
		logError("DragQueryFile(hDrop, 0, m_szFile, MAX_PATH) failed\n");
		hr = E_INVALIDARG;
	}

	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);

	logError("CsqfsExt::Initialize() SUCCESS\n");

	return hr;
}

HMENU CsqfsExt::CreateDrivesMenu(UINT &uidFirstCmd)
{
	cmdMap.clear();
	HMENU hMenu = CreateMenu();
	if (hMenu)
	{
		DWORD dwDrives = ::GetLogicalDrives();
		DWORD mask = (0x00000001 << 25);
		auto firstId = uidFirstCmd;
		for (int i = 25; i >= 0; --i)
		{
			if ((mask & dwDrives) == 0)
			{
				auto cmdId = uidFirstCmd++;
				TCHAR drv[3] = { (TCHAR)(_T('A') + i), _T(':'), _T('\0') };
				cmdMap.emplace(cmdId - firstId, (TCHAR)(_T('a') + i));
				AppendMenu(hMenu, MF_STRING, cmdId, drv);
			}
			mask >>= 1;
		}
		auto cmdId = uidFirstCmd++;
		cmdMap.emplace(cmdId - firstId, '.');
		AppendMenu(hMenu, MF_STRING, cmdId, _T("Browse..."));
	}
	return hMenu;
}


HRESULT CsqfsExt::QueryContextMenu(
	HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
	UINT uidLastCmd, UINT uFlags)
{
	logError("CsqfsExt::QueryContextMenu() CALLED uidFirstCmd== %d, uidLastCmd == %d\n", uidFirstCmd, uidLastCmd);
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if (uFlags & CMF_DEFAULTONLY)
	{
		logError("(uFlags & CMF_DEFAULTONLY) RETURNING\n");
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

	CString str;
	if (FALSE == str.LoadStringW(IDS_MOUNT_ARCHIVE))
	{
		logError("str.LoadString(IDS_MOUNT_ARCHIVE) failed\n");
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, 128);
	}

	if (hDrivesMenu)
	{
		DestroyMenu(hDrivesMenu);
		hDrivesMenu = nullptr;
	}
	hDrivesMenu = CreateDrivesMenu(uidFirstCmd);

	InsertMenu(hmenu, uMenuIndex, MF_BYPOSITION | MF_POPUP,
		(UINT_PTR)hDrivesMenu, (LPCTSTR)str);

	logError("CsqfsExt::QueryContextMenu() SUCCESS\n");
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, uidFirstCmd);
}


HRESULT CsqfsExt::GetCommandString(
	UINT_PTR idCmd, UINT uFlags, UINT* pwReserved,
	LPSTR pszName, UINT cchMax)
{
	USES_CONVERSION;

	// Check idCmd, it must be 0 since we have only one menu item.
	if (0 != idCmd)
		return E_INVALIDARG;

	// If Explorer is asking for a help string, copy our string into the
	// supplied buffer.
	if (uFlags & GCS_HELPTEXT)
	{
		LPCTSTR szText = _T("This is the simple shell extension's help");

		if (uFlags & GCS_UNICODE)
		{
			// We need to cast pszName to a Unicode string, and then use the
			// Unicode string copy API.
			lstrcpynW((LPWSTR)pszName, T2CW(szText), cchMax);
		}
		else
		{
			// Use the ANSI string copy API to return the help string.
			lstrcpynA(pszName, T2CA(szText), cchMax);
		}

		return S_OK;
	}

	return E_INVALIDARG;
}

CString CsqfsExt::GetMappedDrive(WORD cmdIndex)
{
	CString drv{};
	auto it = cmdMap.find(cmdIndex);
	if (it != std::end(cmdMap))
	{
		TCHAR c = it->second;
		if (c >= _T('a') && c <= _T('z'))
		{
			TCHAR tmp[] = { c, _T(':'), _T('\0') };
			drv = tmp;
		}
	}
	return drv;
}

HRESULT CsqfsExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if (0 != HIWORD(pCmdInfo->lpVerb))
		return E_INVALIDARG;

	auto cmdIndex = LOWORD(pCmdInfo->lpVerb);
	logError("CsqfsExt::InvokeCommand() cmdIndex == %d\n", cmdIndex);
	// Get the command index - the only valid one is 0.
	auto drv = GetMappedDrive(cmdIndex);
	if (!drv.IsEmpty())
	{
		MapNetworkDrive(drv, GetArchiveUNCPath());
		return S_OK;
	}
	else
	{
		auto mountPath = GetSqfsMountDirectory(pCmdInfo->hwnd);
		if (MapArchivePath(mountPath, m_szFile))
		{
			return S_OK;
		}
	}

	return E_FAIL;
}

BOOL CsqfsExt::MapNetworkDrive(const CString &mountPoint, const CString &archivePath)
{
	// DBGLOG(m_log << _T("MapNetworkDrive() mountPoint==") << (LPCTSTR)mountPoint << _T("archivePath==") << (LPCTSTR)archivePath);
	NETRESOURCE res;
	res.dwScope = RESOURCE_GLOBALNET;
	res.dwType = RESOURCETYPE_DISK;
	res.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
	res.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	res.lpLocalName = const_cast<LPTSTR>(static_cast<LPCTSTR>(mountPoint));
	res.lpRemoteName = const_cast<LPTSTR>(static_cast<LPCTSTR>(archivePath));
	res.lpComment = NULL;
	res.lpProvider = NULL;
	auto result = WNetAddConnection2(&res, NULL, NULL, 0);
	return (NO_ERROR == result);

}

BOOL CsqfsExt::MapArchivePath(const CString &path, const CString &archivePath)
{
	//DBGLOG(m_log << _T("MapArchivePath() path==") << (LPCTSTR)path << _T("archivePath==") << (LPCTSTR)archivePath);
	auto command = GetLaunchCtlPath();
	command += _T(" start squashfs ");
	command += path + ' ' + archivePath + ' ' + path;
	//DBGLOG(m_log << _T("MapArchivePath() command==") << (LPCTSTR)command);

	STARTUPINFO st{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	st.cb = sizeof(st);
	st.hStdError = CreateFile(_T("nul"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	st.hStdInput = st.hStdError;
	st.hStdOutput = st.hStdError;
	st.dwFlags = STARTF_USESTDHANDLES;
	auto result = CreateProcess(nullptr,
		const_cast<LPTSTR>(static_cast<LPCTSTR>(command)),
		nullptr,
		nullptr,
		FALSE,
		CREATE_BREAKAWAY_FROM_JOB | DETACHED_PROCESS,
		nullptr,
		nullptr,
		&st,
		&pi
	);

	if (result)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		//DBGLOG(m_log << _T("CreateProcess() failed. LastError == ") << GetLastError());
	}
	return (result != 0);
}

#define FSP_LAUNCHERPATH                     FSP_LIBPATH FSP_LAUNCHERNAME

CString CsqfsExt::GetLaunchCtlPath()
{
	WCHAR PathBuf[MAX_PATH];
	DWORD Size;
	HKEY RegKey;
	LONG Result;


	Result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, INSTALL_KEY,
		0, KEY_READ, &RegKey);
	if (ERROR_SUCCESS == Result)
	{
		Size = sizeof PathBuf - sizeof L"" FSP_LAUNCHERPATH + sizeof(WCHAR);
		Result = RegGetValueW(RegKey, 0, L"InstallDir",
			RRF_RT_REG_SZ, 0, PathBuf, &Size);
		RegCloseKey(RegKey);
	}
	if (ERROR_SUCCESS != Result)
		return CString{};

	RtlCopyMemory(PathBuf + (Size / sizeof(WCHAR) - 1), L"" FSP_LAUNCHERPATH, sizeof L"" FSP_LAUNCHERPATH);
	return CString{ PathBuf };
}

CString CsqfsExt::GetArchiveUNCPath()
{
	auto path = CString{ _T("\\\\squashfs\\") };
	CString arch = m_szFile;
	arch.Replace(_T(":"), _T(""));
	path += arch;
	// DBGLOG(m_log << _T("GetArchiveUNCPath() ") << (LPCTSTR)path);
	return path;
}

CString CsqfsExt::GetSqfsMountDirectory(HWND hWnd)
{
	TCHAR fileName[_MAX_PATH];

	CString s = m_szFile;
	int indx = s.ReverseFind('.');
	s = s.Left(indx);

	fileName[0] = _T('\0');
	_tcscpy_s(fileName, (LPCTSTR)s);
	OPENFILENAME ofn = { sizeof(OPENFILENAME),
		hWnd,
		NULL,
		NULL,
		NULL,
		0,
		0,
		fileName,
		_MAX_PATH,
		NULL,
		0,
		NULL,
		_T("Choose Mount Point"),
		OFN_NOCHANGEDIR,
		0,
		0,
		NULL,
		0 };

	if (GetSaveFileName(&ofn))
	{
		// GetDlgItem(IDC_MOUNT_PATH).SetWindowText(ofn.lpstrFile);
		return CString{ ofn.lpstrFile };
	}
	return CString{};
}


// CsqfsExt

