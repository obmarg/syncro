// CSyncroExtension.h : Declaration of the CCSyncroExtension

#pragma once
#include "ExplorerExtension_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>

using namespace ATL;



// CCSyncroExtension

class ATL_NO_VTABLE CCSyncroExtension :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCSyncroExtension, &CLSID_CSyncroExtension>,
	public IDispatchImpl<ISyncroExtension, &IID_ISyncroExtension, &LIBID_ExplorerExtensionLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
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

DECLARE_REGISTRY_RESOURCEID(IDR_CSYNCROEXTENSION)

DECLARE_NOT_AGGREGATABLE(CCSyncroExtension)

BEGIN_COM_MAP(CCSyncroExtension)
	COM_INTERFACE_ENTRY(ISyncroExtension)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()




// ISyncroExtension
public:
};

OBJECT_ENTRY_AUTO(__uuidof(CSyncroExtension), CCSyncroExtension)
