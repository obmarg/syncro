// SyncroAdmin.cpp : Defines the entry point for the console application.
//

#include <kode/base64.h>
#include <cryptopp/sha.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if( vm.count("help") )
	{
		std::cout << desc << "\n";
		return 1;
	}

	std::cout << "Enter password: \n";
	
	std::string password;
	std::cin >> password;

	unsigned char buffer[ CryptoPP::SHA::DIGESTSIZE ];
	CryptoPP::SHA().CalculateDigest(
			buffer,
			reinterpret_cast< const unsigned char* >( password.c_str() ),
			password.length()
			);

	std::cout << kode::base64::Encode( buffer, sizeof( buffer ) ) << "\n";
}

