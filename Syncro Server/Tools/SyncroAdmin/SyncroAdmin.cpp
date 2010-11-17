// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <libsyncro/connection.h>

int main(int argc, char* argv[])
{
	syncro::client::Connection(
		syncro::client::ConnectionDetails()
			.SetUsername("grambo")
			.SetPassword("password")
			.SetHostname("localhost")
		);

	return 0;
}

