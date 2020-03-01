// sqfsExt.h : Declaration of the CsqfsExt

#pragma once
#include "resource.h"       // main symbols
#include "squashfsExt_i.h"
#include <unordered_map>
#include <atlstr.h>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CsqfsExt

class ATL_NO_VTABLE CsqfsExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CsqfsExt, &CLSID_sqfsExt>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CsqfsExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CsqfsExt)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		// hDrivesMenu = CreateDrivesMenu();
		return S_OK;
	}

	void FinalRelease()
	{
		if (hDrivesMenu)
		{
			DestroyMenu(hDrivesMenu);
			hDrivesMenu = nullptr;
		}
	}
private:
	HMENU CreateDrivesMenu(UINT &uidFirstCmd);
	BOOL MapNetworkDrive(const CString &mountPoint, const CString &archivePath);
	BOOL MapArchivePath(const CString &path, const CString &archivePath);
	CString GetLaunchCtlPath();
	CString GetArchiveUNCPath();
	CString GetMappedDrive(WORD cmdIndex);
	CString GetSqfsMountDirectory(HWND hWnd);
public:
	// IShellExtInit
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

protected:
	TCHAR m_szFile[MAX_PATH];
	HMENU hDrivesMenu{ nullptr };
	std::unordered_map<UINT, TCHAR> cmdMap{};
};

OBJECT_ENTRY_AUTO(__uuidof(sqfsExt), CsqfsExt)
