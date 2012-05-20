Requirements
-------------

Boost
Crypto++ 5.6.1 (others may work)
Google Protocol Buffers 2.3.0
Sqlite 3.7.2
cppunit (for testing)
scons


Building on windows
------------
Builds on windows are done with Visual Studio 2010.
Open the Syncro Server solution in VS2010, and configure the include/library paths for your environment.



Building on linux
-------------
Ensure you have all the neccesary dependencies installed.

On debian based systems, this command should install the dependencies:

sudo apt-get install build-essential libboost-all-dev libsqlite3-dev scons libcrypto++-dev libprotobuf-dev protobuf-compiler libcppunit-dev 

Then just run scons from the Syncro Server directory
Binaries will be generated in the bin folder


Building on mac os
-------------
Ensure you have all the neccesary dependencies installed.

Assuming MacPorts is installed, the following command should install the dependencies:

sudo port install boost libcryptopp protobuf-cpp cppunit

Then just run scons from the Syncro Server directory
Binaries will be generated in the bin folder
