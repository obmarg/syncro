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

package uk.me.grambo.syncro.comms;

/**
 * Class to store details of a connection to a server
 * @author Grambo <grambo@grambo.me.uk>
 *
 */
public class ConnectionDetails {
	private String 	m_hostname;
	private int		m_port;
	private String	m_username;
	private String 	m_password;
	
	public static ConnectionDetails createInstance()
	{
		return new ConnectionDetails();
	}
	
	private ConnectionDetails()
	{
	}
	
	/**
	 * @return the m_hostname
	 */
	public String getHostname() {
		return m_hostname;
	}
	/**
	 * @param hostname the hostname to set
	 */
	public ConnectionDetails setHostname(String hostname) {
		this.m_hostname = hostname;
		return this;
	}
	/**
	 * @return the m_port
	 */
	public int getPort() {
		return m_port;
	}
	/**
	 * @param port the port to set
	 */
	public ConnectionDetails setPort(int port) {
		this.m_port = port;
		return this;
	}
	/**
	 * @return the username
	 */
	public String getUsername() {
		return m_username;
	}
	/**
	 * @param m_username the m_username to set
	 */
	public ConnectionDetails setUsername(String username) {
		this.m_username = username;
		return this;
	}
	/**
	 * @return the m_password
	 */
	public String getPassword() {
		return m_password;
	}
	/**
	 * @param m_password the m_password to set
	 */
	public ConnectionDetails setPassword(String password) {
		this.m_password = password;
		return this;
	}
}
