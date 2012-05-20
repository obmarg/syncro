#include <libsyncro/connection.h>
#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace syncro;
using namespace syncro::client;

BOOST_PYTHON_MODULE(pysyncro)
{
    using namespace boost::python;

    class_< FileInfo >( "FileInfo", init< std::string, int64_t >() )
        .def_readonly( "name", &FileInfo::name )
        .def_readonly( "size", &FileInfo::size )
        ;

    class_< FileList >( "FileList" )
        .def( vector_indexing_suite< FileList >() )
        ;

    class_< FolderInfo >( "FolderInfo" )
        .def( init< int, std::string, std::string, std::string >() )
        .def_readonly( "Id", &FolderInfo::Id )
        .def_readonly( "Name", &FolderInfo::Name )
        .def_readonly( "Path", &FolderInfo::Path )
        .def_readonly( "UploadPrefix", &FolderInfo::UploadPrefix )
        ;

    class_< FolderList >( "FolderList" )
        .def( vector_indexing_suite< FolderList >() )
        ;

    class_< ConnectionDetails >( "ConnectionDetails" )
        .def( 
                "SetHostname", 
                &ConnectionDetails::SetHostname, 
                return_value_policy< reference_existing_object >() 
                )
        .def( 
                "SetPort",
                &ConnectionDetails::SetPort,
                return_value_policy< reference_existing_object >() 
                )
        .def( 
                "SetUsername",
                &ConnectionDetails::SetUsername,
                return_value_policy< reference_existing_object >()
                )
        .def( 
                "SetPasswordHash",
                &ConnectionDetails::SetPasswordHash,
                return_value_policy< reference_existing_object >()
                )
        .def( 
                "SetPassword",
                &ConnectionDetails::SetPassword,
                return_value_policy< reference_existing_object >()
                )
        ;

    class_< FileTransferDetails >( "FileTransferDetails" )
        .def( 
                "SetFolderId",
                &FileTransferDetails::SetFolderId,
                return_value_policy< reference_existing_object >()
                )
        .def( 
                "SetRemotePath",
                &FileTransferDetails::SetRemotePath,
                return_value_policy< reference_existing_object >()
                )
        .def( 
                "SetLocalPath",
                &FileTransferDetails::SetLocalPath,
                return_value_policy< reference_existing_object >()
                )
        .def( 
                "SetOneShot",
                &FileTransferDetails::SetOneShot,
                return_value_policy< reference_existing_object >()
                )
        ;

    class_< Connection, boost::noncopyable >(
            "Connection", 
            init< ConnectionDetails >() 
            )
        .def( "GetUuid", &Connection::GetUuid )
        .def( "GetServerName", &Connection::GetServerName )
        .def( "GetFolderList", &Connection::GetFolderList )
        .def( "GetFolderContents", &Connection::GetFolderContents )
        .def( "UploadFile", &Connection::UploadFile )
        .def( "DownloadFile", &Connection::DownloadFile )
        .def( "SendAdminCommand", &Connection::SendAdminCommand )
        ;
}
