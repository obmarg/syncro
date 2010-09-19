#ifndef _FOLDER_H_
#define _FOLDER_H_

#include <string>
#include <vector>
#include <memory>

class CFolder {
public:
	CFolder( const std::string& insPath );
	~CFolder();

	std::string GetName() { return m_sFolderName; }
	std::vector< std::string >& GetFiles() { return m_oFiles; };
	std::vector< std::tr1::shared_ptr<CFolder> >& GetChildren() { return m_oChildren; }
private:
	std::string m_sPath;
	std::string m_sFolderName;
	std::vector< std::tr1::shared_ptr<CFolder> > m_oChildren;
	std::vector< std::string > m_oFiles;
};


#endif