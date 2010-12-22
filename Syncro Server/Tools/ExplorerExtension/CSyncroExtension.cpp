// CSyncroExtension.cpp : Implementation of CCSyncroExtension

#include "stdafx.h"
#include "CSyncroExtension.h"
#include "BackgroundThread.h"
#include <atlconv.h>  // for ATL string conversion macros
#include <libsyncro/connection.h>
#include <libsyncro/folderlist.h>
#include <ShellAPI.h>
#include <boost/foreach.hpp>
#include <iostream>

CCSyncroExtension::CCSyncroExtension()
	:m_background( BackgroundThread::GetInstance() )
{
	
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

		bool addMenu = m_background.GetData(
			boost::bind( &CCSyncroExtension::ServerListCallback, this, _1 )
			);

		if( addMenu )
		{
			HMENU hPopupMenu = CreateMenu();

			//TODO: Ids need to link to actual actions...
			AppendMenu( hPopupMenu, MF_STRING, 0, L"Add Folder");
			AppendMenu( 
				hPopupMenu, 
				MF_STRING | MF_POPUP, 
				reinterpret_cast< UINT_PTR >(m_sendFileMenu),
				L"Send File"
				);

			InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION | MF_STRING | MF_POPUP,
				reinterpret_cast< UINT_PTR >(hPopupMenu), _T("Syncro") );
		}

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
		
		if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
			return E_INVALIDARG;

		int commandId = LOWORD( pCmdInfo->lpVerb );
		if( commandId >= 0 || commandId < m_menuItems.size() )
		{
			if( m_menuItems[ commandId ].Callback )
				m_menuItems[ commandId ].Callback();
		}

		//TODO: perform some actions
		return E_INVALIDARG;
}

void CCSyncroExtension::ServerListCallback( 
	const BackgroundThread::ServerList& list 
	)
{
	using syncro::FolderInfo;
	using syncro::FolderList;

	m_sendFileMenu = CreateMenu();
	m_menuItems.clear();

	if( list.size() == 1 )
	{
		m_menuItems.reserve( list[0].folders.size() );

		std::wstring folderPostfix = 
			L"on " + kode::utils::wstring( list[0].Name );

		const FolderList& folders = list[0].folders;
		BOOST_FOREACH( const FolderInfo& folder, folders )
		{
			//TODO: Associate this menu item with a callback
			//		in some sort of callback list
			std::wstring itemText = 
				kode::utils::wstring( folder.Name ) + folderPostfix;
			AppendMenu( m_sendFileMenu, MF_STRING, m_menuItems.size() , itemText.c_str() );
			m_menuItems.push_back( MenuItemInfo(
				L"Upload file to " + itemText,
				VoidCallback()
				) );
		}
	}
	else
	{
		BOOST_FOREACH( const BackgroundThread::ServerInfo& info, list )
		{
			HMENU serverMenu = CreateMenu();

			std::wstring serverName = kode::utils::wstring( info.Name );
			const FolderList& folders = list[0].folders;
			BOOST_FOREACH( const FolderInfo& folder, folders )
			{
				//TODO: Associate this menu item with a callback
				//		in some sort of callback list
				std::wstring itemText = kode::utils::wstring( folder.Name );
				AppendMenu( serverMenu, MF_STRING, m_menuItems.size(), itemText.c_str() );
				m_menuItems.push_back( MenuItemInfo(
					L"Upload file to " + itemText + L" on " + serverName,
					VoidCallback()
					) );
			}
			AppendMenu(
				m_sendFileMenu, 
				MF_STRING | MF_POPUP,
				reinterpret_cast< UINT_PTR >(serverMenu), 
				serverName.c_str()
				);
		}
	}

}

// CCSyncroExtension

