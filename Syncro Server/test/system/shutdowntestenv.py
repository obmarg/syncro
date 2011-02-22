#!/usr/bin/python

import os
import signal

pidfile = open( 'testsyncro.pid', 'r' )
try:
	pid=int( pidfile.readline() )
	pidfile.close()
	os.kill( pid, signal.SIGHUP )
#except IOError as e:
#	print e
#except OSError as e:
#	print e
except:
	print( "Python exception caught. Ignoring" )
	pass
