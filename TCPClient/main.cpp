#include <string.h>
#include <assert.h>
#include "SocketClient.h"
#include "debug.h"

int main()
{
	SocketClient* socketClient = NULL;
	do {
		if (!socketClient || socketClient->getSocketfd() <= 0) {
			if (socketClient) {
				delete socketClient;
			}
			else {
				socketClient = new SocketClient("192.168.56.101", 8090, true);
				bool success = socketClient->connectServer();
				if (success) {
					printf("ConnectServer success !\n");
					log(stdout, "sendMsg...");
					bool bsend = socketClient->sendMsg((unsigned char*) & "i am client\n", strlen("i am client\n"));
					if (bsend) {
						log(stdout, "sendMsg success");
					}
				}
			}
		}
		else {
			sleep(1);
			log(stdout, "recvMsg...");
			unsigned char buffer[174 * 1024] = { 0x00 };
			int nbytes = socketClient->recvMsg(buffer, 174 * 1024 - 1);
			if (nbytes > 0) {
				log(stdout, "------------------------- msg content ------------------------------- ");
				for (size_t i = 0; buffer[i] != 0x00; i++)
				{
					printf("%c", buffer[i]);
				}
				printf("\n");
				log(stdout, "----------------------------------------------------------------------");
				memset(buffer, 0x00, 174 * 1024 - 1);
			}
			else {
				log(stdout, "recvMsg error");
			}

			log(stdout, "input msg:");
			char sendBuffer[1024] = { 0x00 };
			memset(sendBuffer, 0x00, 1024);
			gets(sendBuffer);
			log(stdout, "sendMsg...");
			bool isend = socketClient->sendMsg((unsigned char*)sendBuffer, strlen(sendBuffer));
			if (isend) {
				log(stdout, "sendMsg success");
			}
			else {
				log(stdout, "sendMsg error");
			}
		}
	} while (true);
	
	return 0;
}