/*
 * Author: lxf
 * E-mail: 1220324958@qq.com
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "meta.h"

class Connector
{
public:
	Connector();
	~Connector();
	int getSocketfd();
	void setSocketfd(int sock_fd);
	struct sockaddr_in& getSockaddr();
	void setSockaddr(struct sockaddr_in& sockaddr);
	bool setNblocking(bool nblock);
	int recvMsg(unsigned char* byteBuf, int nsize);
	bool sendMsg(const unsigned char* byteBuf, int nsize);
	void Shutdown(int how = SHUT_RDWR);
	void Close();

private:
	struct sockaddr_in _sockaddr_in;
	int _sock_fd;
};

#endif

