#!/usr/bin/python

import os
import subprocess
import sys

try:
	os.remove("test.db")
except OSError:
	pass


subprocess.call( ["./SystemTest -c test.db"], shell=True )

pid = 0
try:
	pid = os.fork()
	if not pid:
		os.execl( "./syncro", "-d", "test.db", "-p", "9981" )
except OSError:
	print "Error: Could not fork\n"
	sys.exit( 1 )

pidfile = open( 'testsyncro.pid', 'w' )
pidfile.write( str(pid) )
pidfile.close()

