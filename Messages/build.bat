protoc -I=. --cpp_out "gen/" binarydata.proto
protoc -I=. --java_out "..\Syncro\src" binarydata.proto
protoc -I=. --cpp_out "gen/" header.proto
protoc -I=. --java_out "..\Syncro\src" header.proto
protoc -I=. --cpp_out "gen/" handshake.proto
protoc -I=. --java_out "..\Syncro\src" handshake.proto
protoc -I=. --cpp_out "gen/" admin.proto
protoc -I=. --java_out "..\Syncro\src" admin.proto
protoc -I=. --cpp_out "gen/" folders.proto
protoc -I=. --java_out "..\Syncro\src" folders.proto
move "gen\*.pb.cc" "..\Syncro Server\libsyncro\src\protocol_buffers\"
move "gen\*.pb.h" "..\Syncro Server\libsyncro\include\libsyncro\protocol_buffers\"