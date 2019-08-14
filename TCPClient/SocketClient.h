#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>


class SocketClient
{
public:
	SocketClient(char* ip, unsigned short port, bool nblock = false);
	~SocketClient();
	int getSocketfd();
	bool setNblocking(bool nblock);
	bool connectServer(unsigned long long timeout = 3);
	int recvMsg(unsigned char* byteBuf, int nsize);
	bool sendMsg(const unsigned char* byteBuf, int nsize);
	void Shutdown(int how = SHUT_RDWR);
	void Close();

private:
	inline int createSocket();

private:
	int _sock_fd;
	char* _host_ip;
	unsigned short _host_port;
};

#endif
