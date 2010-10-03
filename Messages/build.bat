protoc -I=. --cpp_out "..\Syncro Server\Syncro Server\protocol_buffers" binarydata.proto
protoc -I=. --java_out "..\Syncro\src" binarydata.proto
protoc -I=. --cpp_out "..\Syncro Server\Syncro Server\protocol_buffers" header.proto
protoc -I=. --java_out "..\Syncro\src" header.proto
