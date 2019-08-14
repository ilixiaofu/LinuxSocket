#include "SocketServer.h"
#include <assert.h>
#include <errno.h>
#include <string.h>

SocketServer::SocketServer(unsigned short port, bool nblock) : _ip(NULL), _port(port)
{
	this->init(nblock);
}

SocketServer::SocketServer(char* ip, unsigned short port, bool nblock) : _ip(ip), _port(port)
{
	this->init(nblock);
}

SocketServer::~SocketServer()
{
	if (this->_ip) {
		delete this->_ip;
	}
	this->Close();
}

void SocketServer::init(bool nblock)
{
	this->_sock_fd = this->CreateSocket();
	this->Bind();
	this->setBlocking(nblock);
}

int SocketServer::CreateSocket()
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(sock_fd > 0);
	return sock_fd;
}

// ����һ���ļ�������Ϊnonblock
bool SocketServer::setBlocking(bool nblock)
{
	if (!nblock) {
		return false;
	}
	else
	{
		assert(this->_sock_fd > 0);
		int fmode = fcntl(this->_sock_fd, F_GETFL, 0);
		if (fmode == -1) {
			fmode = 0;
		}
		int iret = fcntl(this->_sock_fd, F_SETFL, fmode | O_NONBLOCK);
		if (!iret) {
			return true;
		}
		else {
			return false;
		}
	}
}

void SocketServer::Bind()
{
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET; // ʹ��IPv4
	sockaddr.sin_port = htons(this->_port); // �˿�
	if (this->_ip) {
		sockaddr.sin_addr.s_addr = inet_addr(this->_ip); // ��ָ��IP
	}
	else {
		sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // ����ip
	}
	assert(this->_sock_fd > 0);
	int iret = bind(this->_sock_fd, (struct sockaddr*)(&sockaddr), sizeof(sockaddr));
	assert(iret >= 0);
}

void SocketServer::Listen(int maxConnectCount)
{
	assert(maxConnectCount > 0);
	assert(this->_sock_fd > 0);
	int iret = listen(this->_sock_fd, maxConnectCount);
	assert(iret >= 0);
}

//if (client_sock < 0)
	//{
	//	switch (errno)
	//	{
	//		// ���������󣬺���
	//	case EAGAIN:
	//	case ECONNABORTED:
	//	case EINTR:
	//	case EMFILE:
	//		break;
	//		// ���������˳�����
	//	case EFAULT:
	//	case EINVAL:
	//	case ENFILE:
	//	case ENOMEM:
	//		break;
	//		// δ֪�����˳�����
	//	default:
	//		break;
	//	}
	//}

Connector* SocketServer::Accept()
{
	struct sockaddr_in sockaddr;
	socklen_t socklen = sizeof(sockaddr);
	memset(&sockaddr, 0, socklen);
	assert(this->_sock_fd > 0);
	int iret_sockfd = accept(this->_sock_fd, (struct sockaddr*)& sockaddr, &socklen);
	//int iret_sockfd = accept4(this->_sock_fd, (struct sockaddr*) & sockaddr, &socklen, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (iret_sockfd < 0) {
		return NULL;
	}
	Connector* connector = new Connector();
	connector->setSockaddr(sockaddr);
	connector->setSocketfd(iret_sockfd);
	return connector;
}

void SocketServer::Close()
{
	assert(this->_sock_fd > 0);
	int iret = close(this->_sock_fd);
	//assert(iret >= 0);
}


	