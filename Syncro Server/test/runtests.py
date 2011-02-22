#!/usr/bin/python

import os
import subprocess
import sys
import time
import signal

try:
	os.remove("test.db")
except OSError:
	pass


subprocess.call( ["./SystemTest -c test.db"], shell=True )
try:
	os.mkdir( "Test1" )
	os.mkdir( "Test2" )
	os.mkdir( "Test3" )
except OSError:
	print( "Failed to Create Test Dirs" )
	pass

try:
	os.remove( "result.xml" )
except OSError:
	pass

pidfilename = "syncro.test.pid"

try:
	subprocess.call( [
		"./syncro",
		"./syncro",
		"-d",
		"test.db",
		"-p",
		"9981",
		"--nobroadcast",
		"--daemon",
		pidfilename ]
		)
except:
	print "Error: Could not execute syncro\n"
	sys.exit( 1 )

# Wait 5 seconds to give server time to start
time.sleep( 3 )

try:
	pidfile = open( pidfilename, "r" )
	pid = int( pidfile.readline() )
	pidfile.close()
except:
	print "Could not get pid from " + pidfilename + "\n"

try:
	subprocess.check_call( [ "./SystemTest", "-p", "9981", "--prepared" ] )
except:
	print( "Error in SystemTest\n" )
	pass	# TODO: Could handle this

try:
	os.kill( pid, signal.SIGHUP )
except OSError as e:
	print( "Python exception caught Ignoring\n" )
	print( e )
	pass
