#include "Connector.h"

#include <string.h>
#include <assert.h>
#include <errno.h>

Connector::Connector()
{
}

Connector::~Connector()
{
}

int Connector::getSocketfd()
{
	return this->_sock_fd;
}

void Connector::setSocketfd(int sock_fd)
{
	this->_sock_fd = sock_fd;
}

struct sockaddr_in& Connector::getSockaddr()
{
	return this->_sockaddr_in;
}

void Connector::setSockaddr(struct sockaddr_in& sockaddr)
{
	this->_sockaddr_in = sockaddr;
}

// ����һ���ļ�������Ϊnonblock
bool Connector::setNblocking(bool nblock)
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

int Connector::recvMsg(unsigned char* byteBuf, int nsize)
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

bool Connector::sendMsg(const unsigned char* byteBuf, int nsize)
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

/*
SHUT_RD:�Ͽ���������
SHUT_WR:�Ͽ��������
SHUT_RDWR:ͬʱ�Ͽ�I/O����
�ɹ�����0��ʧ�ܷ���-1��
*/
void Connector::Shutdown(int how)
{
	assert(this->_sock_fd > 0);
	int iret = shutdown(this->_sock_fd, how);
	//assert(iret == 0);
	this->_sock_fd = iret;
}
void Connector::Close()
{
	assert(this->_sock_fd > 0);
	int iret = close(this->_sock_fd);
	//assert(iret >= 0);
	this->_sock_fd = iret;
}
