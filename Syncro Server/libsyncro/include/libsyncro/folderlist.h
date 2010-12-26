#ifndef SYNCRO_FOLDERLIST_H_
#define SYNCRO_FOLDERLIST_H_

#include <string>
#include <vector>

namespace syncro {

struct FolderInfo {
	FolderInfo() : Id( -1 ) {};

	FolderInfo( int innID, const std::string& insName ) : 
	Id( innID ), 
	Name( insName ),
	UploadPrefix("")
	{};

	FolderInfo( 
		int innID, 
		const std::string& insName, 
		const std::string& uploadPrefix 
		) : 
	Id( innID ), 
	Name( insName ),
	UploadPrefix( uploadPrefix )
	{};

	int Id;
	std::string Name;
	std::string UploadPrefix;
};

typedef std::vector< FolderInfo > FolderList;

}	// namespace syncro

#endif
