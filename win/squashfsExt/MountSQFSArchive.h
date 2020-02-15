#pragma once
#ifdef OBSOLETE

#include "resource.h"
#include <atlhost.h>
#include <atlstr.h>
#include <fstream>

using namespace ATL;

// CMountSQFSArchive

class CMountSQFSArchive :
	public CAxDialogImpl<CMountSQFSArchive>
{
public:
	CMountSQFSArchive(LPCTSTR lpszArchive)
		: m_archive(lpszArchive)
	{
#ifdef _DEBUG
		m_log.open("C:\\Users\\dslat\\squashfsExt.log", std::ios_base::in | std::ios_base::out| std::ios_base::trunc);
#endif
	}

	~CMountSQFSArchive()
	{
#ifdef _DEBUG
		m_log.flush();
		m_log.close();
#endif
	}

	enum { IDD = IDD_MOUNT_SQUASHFS };
private:

	BOOL EnableEditControl(BOOL enable);
	BOOL EnableDriveLetterCombo(BOOL enable);
	BOOL SetRadioIndex(int index);
	BOOL EnableChooseDirectoryBtn(BOOL enable);

	CString GetArchiveMountPoint();
	CString GetDestinationDrive();
	CString GetDestinationPath();
	CString GetArchiveUNCPath();
	LRESULT MapNetworkDrive();
	LRESULT MapArchivePath();

private:
	CString	m_archive;
#ifdef _DEBUG
	std::wfstream	m_log;
#endif

public:

BEGIN_MSG_MAP(CMountSQFSArchive)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CMountSQFSArchive>)
	COMMAND_HANDLER(IDC_CHOOSE_DRIVE, CBN_SELCHANGE, OnCbnSelchangeDriveLetter)
	COMMAND_HANDLER(IDC_CHOOSEDIR_BUTTON, BN_CLICKED, OnBnClickedChooseDir)
	COMMAND_HANDLER(IDC_MAP_DRIVE_LETTER, BN_CLICKED, OnBnClickedMapDriveLetter)
	COMMAND_HANDLER(IDC_CHOOSE_DIRECTORY, BN_CLICKED, OnBnClickedChooseDirectory)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnBnClickedMapDriveLetter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeDriveLetter(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChooseDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChooseDirectory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
#ifdef _DEBUG
#define DBGLOG(a) if(m_log) {a << '\n';}
#else
#define DBGLOG(a) ((void)0)
#endif

#endif // OBSOLETE
