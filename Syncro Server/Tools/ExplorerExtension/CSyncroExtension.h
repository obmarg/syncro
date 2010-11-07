// CSyncroExtension.h : Declaration of the CCSyncroExtension

#pragma once
#include "ExplorerExtension_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <ShlObj.h>

using namespace ATL;



// CCSyncroExtension

class ATL_NO_VTABLE CCSyncroExtension :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCSyncroExtension, &CLSID_CSyncroExtension>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CCSyncroExtension()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	STDMETHODIMP GetCommandString(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

DECLARE_REGISTRY_RESOURCEID(IDR_CSYNCROEXTENSION)

DECLARE_NOT_AGGREGATABLE(CCSyncroExtension)

BEGIN_COM_MAP(CCSyncroExtension)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()




// ISyncroExtension
public:
};

OBJECT_ENTRY_AUTO(__uuidof(CSyncroExtension), CCSyncroExtension)
