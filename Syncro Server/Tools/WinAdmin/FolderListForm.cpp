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

#include "stdafx.h"
#include "FolderListForm.h"
#include "EditFolderDetails.h"
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace WinAdmin
{

std::string string( System::String ^ input )
{
	cli::array<wchar_t, 1> ^ characters = input->ToCharArray();
	std::stringstream stream;
	for( int i = 0; i < input->Length; i++ )
	{
		stream << boost::numeric_cast<char>( characters[i] );
	}
	return stream.str();
}

void FolderListForm::InitialSetup()
{
	using namespace syncro::client;

	Connection conn(
	    ConnectionDetails()
	    .SetHostname( "localhost" )
	    .SetUsername( "admin" )
	    .SetPassword( "password" )
	);

	SetupList( conn );
}

void FolderListForm::SetupList( syncro::client::Connection& conn )
{
	syncro::FolderList folders;
	conn.GetFolderList( folders );

	lstFolders->BeginUpdate();
	lstFolders->Items->Clear();
	m_folderIds = gcnew cli::array<unsigned int>( folders.size() );

	int i = 0;
	BOOST_FOREACH( const syncro::FolderInfo & folder, folders )
	{
		lstFolders->Items->Add( gcnew String( folder.Name.c_str() ) );
		m_folderIds[ i ] = folder.Id;
		++i;
	}
	lstFolders->EndUpdate();
}

void FolderListForm::StartAddFolder()
{
	using namespace syncro::client;

	EditFolderDetails ^ addFolderDialog = gcnew EditFolderDetails();
	addFolderDialog->ShowDialog( this );
	if( addFolderDialog->Ok() )
	{
		Connection conn(
		    ConnectionDetails()
		    .SetHostname( "localhost" )
		    .SetUsername( "admin" )
		    .SetPassword( "password" )
		);

		Connection::StringMap params;
		params.insert( std::make_pair(
		                   "path",
		                   string( addFolderDialog->GetPath() )
		               ) );
		params.insert( std::make_pair(
		                   "name",
		                   string( addFolderDialog->GetName() )
		               ) );

		conn.SendAdminCommand( "AddFolder", params );
		SetupList( conn );
	}
}

void FolderListForm::StartEditFolder()
{
}

void FolderListForm::DeleteFolder( unsigned int id )
{
	using namespace syncro::client;

	Connection conn(
	    ConnectionDetails()
	    .SetHostname( "localhost" )
	    .SetUsername( "admin" )
	    .SetPassword( "password" )
	);
	Connection::StringMap params;
	params.insert( std::make_pair(
	                   "id",
	                   boost::lexical_cast< std::string >( id )
	               ) );
	conn.SendAdminCommand(
	    "DelFolder",
	    params
	);
	SetupList( conn );
}

}	// namespace WinAdmin