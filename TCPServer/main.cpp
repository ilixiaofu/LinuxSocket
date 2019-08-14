#include <cstdio>
#include <string.h>
#include "SocketServer.h"
#include "debug.h"

int main()
{
	SocketServer socketServer(8090);
	socketServer.Listen();
	Connector* connector = NULL;
	fd_set fds;
	struct timeval tv;
	do {
		if (!connector || connector->getSocketfd() <= 0 ) {
			if (connector) {
				delete connector;
			}
			else {
				log(stdout, "wait client to connect...");
				connector = socketServer.Accept();
				char* ip = inet_ntoa(connector->getSockaddr().sin_addr);
				unsigned int port = ntohs(connector->getSockaddr().sin_port);
				fprintf(stdout, "ip:%s, port:%d connected\n", ip, port);
				connector->setNblocking(true);
				connector->sendMsg((unsigned char*)&"hello i am server\n",strlen("hello i am server\n"));
			}
		}
		else {
			sleep(1);
			/* �Ѽ������ */
			FD_ZERO(&fds);
			/* �ѱ�׼������0���뵽������ */
			FD_SET(0, &fds);
			FD_SET(connector->getSocketfd(), &fds);
			/* �������ȴ�ʱ�� */
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			/* ��ʼ�ȴ� */
			int retval = select(connector->getSocketfd() + 1, &fds, NULL, NULL, &tv);

			if (retval < 0) {
				log(stdout, "client break connect");
			}
			else if (retval == 0) {
				log(stdout, "wait recv msg...");
				continue;
			}
			else {
				if (FD_ISSET(connector->getSocketfd(), &fds)) {
					unsigned char buffer[174 * 1024] = { 0x00 };
					int nrecv = connector->recvMsg(buffer, 174 * 1024 - 1);
					if (nrecv > 0) {
						char* ip = inet_ntoa(connector->getSockaddr().sin_addr);
						printf("%s:", ip);
						for (size_t i = 0; i < nrecv; i++)
						{
							printf("%c", buffer[i]);
						}
						printf("\n");
						sprintf((char*)buffer, "%s\n", buffer);
						connector->sendMsg(buffer, 174 * 1024 - 1);
						memset(buffer, 0x00, 174 * 1024 - 1);
					}
				}
			}
		}
	} while (true);
	socketServer.Close();

	return 0;
}