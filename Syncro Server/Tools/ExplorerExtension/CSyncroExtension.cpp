// CSyncroExtension.cpp : Implementation of CCSyncroExtension

#include "stdafx.h"
#include "CSyncroExtension.h"
#include <atlconv.h>  // for ATL string conversion macros
#include <libsyncro/connection.h>
#include <ShellAPI.h>

CCSyncroExtension::CCSyncroExtension()
{
	syncro::client::Connection conn(
		syncro::client::ConnectionDetails()
			.SetHostname("localhost")
			.SetUsername("grambo")
			.SetPassword("password")
		);
	//If we got here, we're sorted.
}

STDMETHODIMP CCSyncroExtension::Initialize ( 
	LPCITEMIDLIST pidlFolder,
	LPDATAOBJECT pDataObj,
	HKEY hProgID ) {
		FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT,
			-1, TYMED_HGLOBAL };
		STGMEDIUM stg = { TYMED_HGLOBAL };
		HDROP     hDrop;

		// Look for CF_HDROP data in the data object. If there

		// is no such data, return an error back to Explorer.

		if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
			return E_INVALIDARG;

		// Get a pointer to the actual data.

		hDrop = (HDROP) GlobalLock ( stg.hGlobal );

		// Make sure it worked.

		if ( NULL == hDrop )
			return E_INVALIDARG;

		// Sanity check  make sure there is at least one filename.

		UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
		HRESULT hr = S_OK;

		if ( 0 == uNumFiles )
		{
			GlobalUnlock ( stg.hGlobal );
			ReleaseStgMedium ( &stg );
			return E_INVALIDARG;
		}

		// Get the name of the first file and store it in our

		// member variable m_szFile.
		TCHAR file[ MAX_PATH ];
		if ( 0 == DragQueryFile ( hDrop, 0, file, MAX_PATH ) )
			hr = E_INVALIDARG;

		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg );

		return hr;
}

HRESULT CCSyncroExtension::QueryContextMenu (
	HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,
	UINT uidLastCmd, UINT uFlags ) {
		// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.

		if ( uFlags & CMF_DEFAULTONLY )
			return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );

		HMENU hPopupMenu = CreateMenu();

		AppendMenu( hPopupMenu, MF_STRING, 0, L"Add Folder");
		AppendMenu( hPopupMenu, MF_STRING, 1, L"Send File To Folder");
		AppendMenu( hPopupMenu, MF_STRING, 2, L"Send File Once");

		InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION | MF_STRING | MF_POPUP,
			reinterpret_cast< UINT_PTR >(hPopupMenu), _T("Syncro") );

		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}

HRESULT CCSyncroExtension::GetCommandString (
	UINT idCmd, UINT uFlags, UINT* pwReserved,
	LPSTR pszName, UINT cchMax ) {
		//TODO: In this function, return help strings for each of the options

		// Check idCmd, it must be 0 since we have only one menu item.

		if ( 0 != idCmd )
			return E_INVALIDARG;

		// If Explorer is asking for a help string, copy our string into the

		// supplied buffer.

		if ( uFlags & GCS_HELPTEXT )
		{
			LPCTSTR szText = _T("This is the simple shell extension's help");

			if ( uFlags & GCS_UNICODE )
			{
				// We need to cast pszName to a Unicode string, and then use the

				// Unicode string copy API.

				lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
			}
			else
			{
				// Use the ANSI string copy API to return the help string.

				//TODO: Fix this bit
				//lstrcpynA ( pszName, T2CA(szText), cchMax );
				//lstrcpynA ( pszName, T2CA(szText), cchMax );
			}

			return S_OK;
		}

		return E_INVALIDARG;
}

HRESULT CCSyncroExtension::InvokeCommand (
	LPCMINVOKECOMMANDINFO pCmdInfo ) {
		//TODO: perform some actions
		 return E_INVALIDARG;
}

// CCSyncroExtension

