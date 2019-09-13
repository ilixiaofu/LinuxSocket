/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "meta.h"
#include "Connector.h"

class SocketServer
{
public:
	SocketServer(unsigned short port, bool nblock = false);
	SocketServer(char* ip, unsigned short port, bool nblock = false);
	~SocketServer();

	bool setBlocking(bool nblock);
	void Listen(int maxConnectCount = 5);
	Connector* Accept();
	void Close();

private:
	inline void init(bool nblock);
	inline int CreateSocket();
	void Bind();

private:
	int _sock_fd;
	char* _ip;
	unsigned short _port;
};

#endif