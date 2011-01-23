/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

// CSyncroExtension.h : Declaration of the CCSyncroExtension

#pragma once
#include "ExplorerExtension_i.h"
#include "resource.h"       // main symbols
#include <comsvcs.h>
#include <ShlObj.h>
#include <boost/function.hpp>

#include "BackgroundThread.h"

using namespace ATL;

typedef boost::function< void () > VoidCallback;

// CCSyncroExtension

class ATL_NO_VTABLE CCSyncroExtension :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCSyncroExtension, &CLSID_CSyncroExtension>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CCSyncroExtension();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	STDMETHODIMP Initialize( LPCITEMIDLIST, LPDATAOBJECT, HKEY );
	STDMETHODIMP GetCommandString( UINT, UINT, UINT*, LPSTR, UINT );
	STDMETHODIMP InvokeCommand( LPCMINVOKECOMMANDINFO );
	STDMETHODIMP QueryContextMenu( HMENU, UINT, UINT, UINT, UINT );

	DECLARE_REGISTRY_RESOURCEID( IDR_CSYNCROEXTENSION )

	DECLARE_NOT_AGGREGATABLE( CCSyncroExtension )

	BEGIN_COM_MAP( CCSyncroExtension )
	COM_INTERFACE_ENTRY( IShellExtInit )
	COM_INTERFACE_ENTRY( IContextMenu )
	END_COM_MAP()

private:
	struct MenuItemInfo
	{
		MenuItemInfo(
		    const std::wstring& inHelpText,
		    const VoidCallback& inCallback
		) :
			HelpText( inHelpText ),
			Callback( inCallback )
		{

		}
		std::wstring HelpText;
		VoidCallback Callback;
	};
	typedef std::vector<MenuItemInfo> MenuItemList;

	std::string m_currentFile;

	BackgroundThread& m_background;
	HMENU m_sendFileMenu;
	MenuItemList m_menuItems;

	void ServerListCallback( const BackgroundThread::ServerList& list );

// ISyncroExtension
public:
};

OBJECT_ENTRY_AUTO( __uuidof( CSyncroExtension ), CCSyncroExtension )
