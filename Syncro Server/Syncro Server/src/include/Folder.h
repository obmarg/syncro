#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace syncro
{

class CFolder
{
public:
	struct sFileData
	{
		sFileData(
		    const std::string& insName,
		    const unsigned int innSize
		) :
			name( insName ),
			size( innSize )
		{ };

		std::string name;
		unsigned int size;
	};

	typedef std::vector< sFileData > TFileList;

	CFolder( const std::string& insPath, const std::string& name );
	~CFolder();

	std::string GetName()
	{
		return m_sFolderName;
	}
	std::string GetPath()
	{
		return m_sPath;
	}
	const TFileList& GetFiles()
	{
		return m_oFiles;
	};
	std::vector< boost::shared_ptr<CFolder> >& GetChildren()
	{
		return m_oChildren;
	}

	void AddFile( const std::string& name, unsigned int size );

private:
	std::string m_sPath;
	std::string m_sFolderName;
	std::vector< boost::shared_ptr<CFolder> > m_oChildren;
	TFileList m_oFiles;
};

}


#endif
