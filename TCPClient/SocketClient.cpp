#include "SocketClient.h"
#include <string.h>
#include <assert.h>
#include <errno.h>

SocketClient::SocketClient(char* ip, unsigned short port, bool nblock) :_host_ip(ip), _host_port(port), _sock_fd(0)
{
	this->_sock_fd = this->createSocket();
	this->setNblocking(nblock);
}
SocketClient::~SocketClient()
{
	if (this->_host_ip) {
		delete this->_host_ip;
	}
	this->Close();
}

int SocketClient::getSocketfd()
{
	return this->_sock_fd;
}

void SocketClient::Shutdown(int how)
{
	assert(this->_sock_fd > 0);
	int iret = shutdown(this->_sock_fd, how);
	//assert(iret == 0);
	this->_sock_fd = iret;
}

void SocketClient::Close()
{
	assert(this->_sock_fd > 0);
	int iret = close(this->_sock_fd);
	//assert(ret == 0);
	this->_sock_fd = iret;
}

// �����׽���
int SocketClient::createSocket()
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	assert(sock_fd > 0);
	return sock_fd;
}
// ����һ���ļ�������Ϊnonblock
bool SocketClient::setNblocking(bool nblock)
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

// ���ӵ�������
bool SocketClient::connectServer(unsigned long long timeout)
{
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
	server_addr.sin_addr.s_addr = inet_addr(this->_host_ip);  //�����IP��ַ
	server_addr.sin_port = htons(this->_host_port);  //�˿�
	unsigned long long connect_timeout = 0;
	do {
		assert(this->_sock_fd > 0);
		int ret = connect(this->_sock_fd, (struct sockaddr*) & server_addr, sizeof(server_addr));
		if (ret >= 0) {
			return true;
		}
		else {
			sleep(1);
			connect_timeout++;
			if (connect_timeout > timeout) {
				connect_timeout = 0;
				return false;
			}
		}
	} while (true);
	return true;
}

/*���շ������Ϣ*/
int SocketClient::recvMsg(unsigned char* byteBuf, int nsize)
{
	assert(byteBuf);
	assert(nsize > 0);
	assert(this->_sock_fd > 0);
	unsigned char* buffer = byteBuf;
	int nread = read(this->_sock_fd, buffer, nsize);
	if (nread < 0) {
		switch (errno) {
		case EAGAIN: // ������������
			return nread;
		case EINTR:// �жϼ�����ȡ //nonblock
			break;
		default:
			this->Shutdown(SHUT_RD);
			return nread;
		}
	}
	return nread;
}

/*�����˷�����Ϣ*/
bool SocketClient::sendMsg(const unsigned char* byteBuf, int nsize)
{
	if (byteBuf && nsize > 0 && this->_sock_fd > 0) {
		const unsigned char* buffer = byteBuf;
		int limit = 1024 * 4;
		int nsend = 0;
		while (nsend < nsize) {
			limit = (nsize - nsend) < limit ? (nsize - nsend) : limit;
			int nwrite = write(this->_sock_fd, (buffer + nsend), limit);
			if (nwrite > 0) {
				nsend += nwrite;
			}
			else if (nwrite == 0) {
				return true;
			}
			else { // nonblock
				switch (errno) {
				case EAGAIN: // ����������
					break;
				case EINTR:// �жϼ�����ȡ
					break;
				default:
					this->Shutdown(SHUT_WR);
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}