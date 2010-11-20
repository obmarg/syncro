// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <libsyncro/connection.h>

int main(int argc, char* argv[])
{
	using namespace syncro::client;

	Connection conn(
		ConnectionDetails()
			.SetUsername( "grambo" )
			.SetPassword( "password" )
			.SetHostname( "localhost" )
		);

	syncro::FolderList list;

	conn.GetFolderList( list );

	int i = 0;
	i += 1;

	return 0;
}

