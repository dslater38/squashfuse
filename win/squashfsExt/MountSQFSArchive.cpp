#include "pch.h"

#ifdef OBSOLETE

#include "MountSQFSArchive.h"
#include <windowsx.h>

namespace std
{
	ostream &operator<<(ostream &o, const CString &s)
	{
		o << static_cast<LPCTSTR>(s);
		return o;
	}
	ostream &operator<<(ostream &&o, const CString &s)
	{
		o << static_cast<LPCTSTR>(s);
		return o;
	}
}

// CMountSQFSArchive

LRESULT CMountSQFSArchive::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CMountSQFSArchive>::OnInitDialog(uMsg, wParam, lParam, bHandled);
	HWND hwndChooseDrive = GetDlgItem(IDC_MAP_DRIVE_LETTER);
	EnableEditControl(FALSE);
	EnableDriveLetterCombo(TRUE);
	SetRadioIndex(0);	
	EnableChooseDirectoryBtn(FALSE);

	DWORD dwDrives = ::GetLogicalDrives();
	DWORD mask = (0x00000001 << 25);
	for (int i = 25; i >= 0; --i)
	{
		if ((mask & dwDrives) == 0)
		{
			TCHAR drv[3] = { (TCHAR)(_T('A') + i), _T(':'), _T('\0') };
			SendDlgItemMessage(IDC_CHOOSE_DRIVE, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)drv);
		}
		mask >>= 1;
	}
	SendDlgItemMessage(IDC_CHOOSE_DRIVE, CB_SETCURSEL, 0, 0);

	GetDlgItem(IDC_LABEL_ARCHIVE).SetWindowText(m_archive);

	bHandled = TRUE;
	return 1;  // Let the system set the focus
}

BOOL CMountSQFSArchive::EnableEditControl(BOOL enable)
{
	return GetDlgItem(IDC_MOUNT_PATH).EnableWindow(enable);
}

BOOL CMountSQFSArchive::SetRadioIndex(int index)
{
	return CheckRadioButton(IDC_MAP_DRIVE_LETTER, IDC_CHOOSE_DIRECTORY, index == 0 ? IDC_MAP_DRIVE_LETTER : IDC_CHOOSE_DIRECTORY);
}

BOOL CMountSQFSArchive::EnableDriveLetterCombo(BOOL enable)
{
	return GetDlgItem(IDC_CHOOSE_DRIVE).EnableWindow(enable);
}

BOOL CMountSQFSArchive::EnableChooseDirectoryBtn(BOOL enable)
{
	return  GetDlgItem(IDC_CHOOSEDIR_BUTTON).EnableWindow(enable);
}

LRESULT CMountSQFSArchive::OnBnClickedMapDriveLetter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EnableEditControl(FALSE);
	EnableDriveLetterCombo(TRUE);
	EnableChooseDirectoryBtn(FALSE);

	return 0;
}

LRESULT CMountSQFSArchive::OnBnClickedChooseDirectory(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EnableEditControl(TRUE);
	EnableDriveLetterCombo(FALSE);
	EnableChooseDirectoryBtn(TRUE);
	return 0;
}


LRESULT CMountSQFSArchive::OnCbnSelchangeDriveLetter(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	return 0;
}


LRESULT CMountSQFSArchive::OnBnClickedChooseDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR fileName[_MAX_PATH];

	CString s = m_archive;
	int indx = s.ReverseFind('.');
	s = s.Left(indx);
	
	fileName[0] = _T('\0');
	_tcscpy_s(fileName, (LPCTSTR)s);
	OPENFILENAME ofn = { sizeof(OPENFILENAME),
		(HWND)(*this),
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
		GetDlgItem(IDC_MOUNT_PATH).SetWindowText(ofn.lpstrFile);
	}
	return 0;
}

CString CMountSQFSArchive::GetDestinationDrive()
{
	CString drive{ "" };
	auto index = SendDlgItemMessage(IDC_CHOOSE_DRIVE, CB_GETCURSEL);
	if (index != CB_ERR)
	{
		TCHAR buffer[3] = { '\0' };
		SendDlgItemMessage(IDC_CHOOSE_DRIVE, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buffer));
		drive = buffer;
	}
	DBGLOG(m_log << _T("GetDestinationDrive() returns ") << (LPCTSTR)drive);
	return drive;
}

CString CMountSQFSArchive::GetDestinationPath()
{
	CString path{};
	GetDlgItemText(IDC_MOUNT_PATH, path);
	DBGLOG(m_log << _T("GetDestinationPath() returns ") << (LPCTSTR)path);
	return path;
}

CString CMountSQFSArchive::GetArchiveMountPoint()
{
	if (IsDlgButtonChecked(IDC_CHOOSEDIR_BUTTON))
	{
		return GetDestinationDrive();
	}
	else if (IsDlgButtonChecked(IDC_CHOOSE_DIRECTORY))
	{
		return GetDestinationPath();
	}
	return _T("");
}

CString CMountSQFSArchive::GetArchiveUNCPath()
{
	auto path = CString{_T("\\\\squashfs\\")};
	auto arch = m_archive;
	arch.Replace(_T(":"), _T(""));
	path += arch;
	DBGLOG(m_log << _T("GetArchiveUNCPath() ") << (LPCTSTR)path);
	return path;
}

LRESULT CMountSQFSArchive::MapNetworkDrive()
{
	auto mountPoint = GetArchiveMountPoint();
	auto archivePath = GetArchiveUNCPath();
	DBGLOG(m_log << _T("MapNetworkDrive() mountPoint==") << (LPCTSTR)mountPoint << _T("archivePath==") << (LPCTSTR)archivePath);
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

LRESULT CMountSQFSArchive::MapArchivePath()
{
	auto path = GetDestinationPath();
	auto archivePath = m_archive;
	DBGLOG(m_log << _T("MapArchivePath() path==") << (LPCTSTR)path << _T("archivePath==") << (LPCTSTR)archivePath);
	CString command{ _T("C:\\Program Files (x86)\\WinFsp\\bin\\launchctl-x64.exe start squashfs ")};
	command += path + ' ' + archivePath + ' ' + path;
	DBGLOG(m_log << _T("MapArchivePath() command==") << (LPCTSTR)command);

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
				CREATE_BREAKAWAY_FROM_JOB| DETACHED_PROCESS,
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
		DBGLOG(m_log << _T("CreateProcess() failed. LastError == ") << GetLastError());
	}
	return (result != 0);
}

LRESULT CMountSQFSArchive::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LRESULT retVal = 0;
	if (IsDlgButtonChecked(IDC_MAP_DRIVE_LETTER))
	{
		retVal = MapNetworkDrive();
	}
	else
	{
		retVal = MapArchivePath();
	}

	EndDialog(wID);
	return 0;
}
#endif // OBSOLETE
