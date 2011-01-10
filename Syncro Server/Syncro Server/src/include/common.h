#ifndef _SYNCRO_COMMON_H_
#define _SYNCRO_COMMON_H_

#include <kode/utils.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <boost/function.hpp>
#include <vector>
#include <stdexcept>

namespace syncro {

struct TCharBuffer {
	typedef unsigned char TChar;
	typedef std::vector<TChar> TBuff;
	TCharBuffer(TBuff& inoBuff, unsigned int innSize) : aBuffer(inoBuff), nSize( innSize ) {};
	
	TBuff::iterator begin() { return aBuffer.begin(); };
	TBuff::iterator end() { return aBuffer.begin() + nSize; };

	TBuff::const_iterator begin() const { return aBuffer.begin(); };
	TBuff::const_iterator end() const { return aBuffer.begin() + nSize; };

	TBuff& aBuffer;
	unsigned int nSize;
};

class authentication_exception : public std::runtime_error {
public:
	authentication_exception(const char* inpError) : runtime_error(inpError) {};
};

class CAuthToken {
public:
	enum AccessLevel {
		AccessLevel_Admin,
		AccessLevel_Normal
	};

	CAuthToken() :
		m_fInitialised( false )
		{};

	CAuthToken(const std::string sUsername, const int innID, const AccessLevel innAccessLevel) : 
		m_sUsername( sUsername ), 
		m_nUserID( innID ),
		m_eAccessLevel( innAccessLevel ),
		m_fInitialised( true )
		{};

	const std::string GetUsername() const {
		return m_sUsername;
	}

	const int GetUserID() const {
		return m_nUserID;
	}
	
	const AccessLevel GetAccessLevel() const {
		return m_eAccessLevel;
	}

	const bool IsInitialised() const {
		return m_fInitialised;
	}

private:
	std::string m_sUsername;
	int m_nUserID;
	AccessLevel m_eAccessLevel;
	bool m_fInitialised;
};

using google::protobuf::io::ZeroCopyInputStream;

typedef std::vector<ZeroCopyInputStream*> TInputStreamList;

typedef boost::function< void () > VoidCallback;

}; //namespace syncro

#endif
