#ifndef SYNCRO_FOLDERLIST_H_
#define SYNCRO_FOLDERLIST_H_

#include <string>
#include <vector>

namespace syncro {

struct FolderInfo {
	FolderInfo() : Id( -1 ) {};

	FolderInfo( int innID, const std::string& insName, const std::string& insPath ) : 
	Id( innID ), 
	Name( insName ),
	Path( insPath ),
	UploadPrefix("")
	{};

	FolderInfo( 
		int innID, 
		const std::string& insName, 
		const std::string& insPath,
		const std::string& uploadPrefix 
		) : 
	Id( innID ), 
	Name( insName ),
	Path( insPath ),
	UploadPrefix( uploadPrefix )
	{};

	int Id;
	std::string Name;
	std::string Path;
	std::string UploadPrefix;
};

typedef std::vector< FolderInfo > FolderList;

}	// namespace syncro

#endif
