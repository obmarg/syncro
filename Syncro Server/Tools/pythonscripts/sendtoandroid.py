#!/usr/bin/python
import pysyncro
import os
import sys

if __name__ == '__main__':
    if len( sys.argv ) != 3:
        if len( sys.argv ) != 2:
            print "Usage: sendtoandroid.py <filename> [ <remote filename> ]"
            quit()
        else:
            remoteFname = os.path.basename( sys.argv[1] )
    else:
        remoteFname = sys.argv[2]
    fname = sys.argv[1]
    print "LocalPath: %s" % fname
    print "RemotePath: %s" % remoteFname
    if not os.path.exists( fname ):
        print "Local file does not exist"
        quit()
    cd = pysyncro.ConnectionDetails()
    cd.SetHostname( "localhost" )
    cd.SetUsername( "admin" ).SetPassword( "password" )
    c = pysyncro.Connection( cd )
    fl = pysyncro.FolderList()
    c.GetFolderList( fl )
    if len( fl ) == 0:
        print "Server has no folders"
        quit()
    print "Uploading to: %s" % fl[0].Name
    targetId = fl[0].Id
    ftd = pysyncro.FileTransferDetails()
    ftd.SetFolderId( fl[0].Id )
    ftd.SetRemotePath( remoteFname ).SetLocalPath( fname )
    c.UploadFile( ftd )
    print "File Uploaded!"
