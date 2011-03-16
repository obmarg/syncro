/*  This file is part of Syncro. 
	Copyright (c) Graeme Coupar <grambo@grambo.me.uk>

	Syncro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Syncro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Syncro.  If not, see <http://www.gnu.org/licenses/>.
*/

package uk.me.grambo.syncro;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.util.Base64;

public class HashUtils {
	public static class HashException extends Exception
	{
		/**
		 * 
		 */
		private static final long serialVersionUID = 1574266974599641093L;
		public HashException( String string )
		{
			super(string);
		}
	}
	
	
	public static String GetFileHash( File inoFile, long size ) throws HashException
	{
		try {
			if( size > inoFile.length() )
			{
				throw new HashException( "Invalid size passed to GetFileHash" );
			}
			MessageDigest digester;
			try {
				digester = MessageDigest.getInstance("SHA-1");
			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
				return "";
			}
			byte[] buffer = new byte[ 1024 * 8 ];
			int read;
			FileInputStream hashInputStream = new FileInputStream( inoFile );
			int nextRead = buffer.length;
			if( nextRead > size )
				nextRead = (int)size;
			while( ( read = hashInputStream.read( buffer, 0, nextRead ) ) > 0 )
			{
				size -= read;
				digester.update(buffer, 0, read);
				if( nextRead > size )
					nextRead = (int)size;
				if( size == 0 )
					break;
			}
			hashInputStream.close();
			byte[] hash = digester.digest();
			return Base64.encodeToString(hash, Base64.DEFAULT );
		}
		catch(IOException e )
		{
			return "";
		}
	}
	
	public static String GetFileHash( File inoFile ) throws Exception
	{
		return GetFileHash( inoFile, inoFile.length() );
	}

}
