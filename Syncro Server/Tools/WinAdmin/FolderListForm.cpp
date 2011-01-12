#include "stdafx.h"
#include "FolderListForm.h"
#include "EditFolderDetails.h"
#include <boost/numeric/conversion/cast.hpp>
#include <string>

namespace WinAdmin
{

std::string string( System::String^ input )
{
	cli::array<wchar_t,1>^ characters = input->ToCharArray();
	std::stringstream stream;
	for( int i=0; i< input->Length; i++ )
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
			.SetHostname("localhost")
			.SetUsername("grambo")
			.SetPassword("password")
		);

	SetupList( conn );
}

void FolderListForm::SetupList( syncro::client::Connection& conn )
{
	syncro::FolderList folders;
	conn.GetFolderList( folders );

	lstFolders->BeginUpdate();
	lstFolders->Items->Clear();
	BOOST_FOREACH( const syncro::FolderInfo& folder, folders )
	{
		lstFolders->Items->Add( gcnew String( folder.Name.c_str() ) );
	}
	lstFolders->EndUpdate();
}

void FolderListForm::StartAddFolder()
{
	using namespace syncro::client;

	EditFolderDetails^ addFolderDialog = gcnew EditFolderDetails();
	addFolderDialog->ShowDialog(this);
	if( addFolderDialog->Ok() )
	{
		Connection conn(
			ConnectionDetails()
				.SetHostname("localhost")
				.SetUsername("grambo")
				.SetPassword("password")
			);
		
		std::string folderString = string( addFolderDialog->GetPath() );
		conn.SendAdminCommand( "AddFolder", folderString );
		SetupList( conn );
	}
}

void FolderListForm::StartEditFolder()
{
}

void FolderListForm::DeleteFolder()
{

}

}	// namespace WinAdmin