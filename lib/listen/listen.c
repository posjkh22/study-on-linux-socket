
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 128

int port_number = 1994;

void getopts(int argc, char* argv[])
{
	int cnt = 1;
	while( cnt < argc )
	{
		if( !strcmp(argv[cnt], "--port"))
		{
			if ( ++cnt < argc )
			{
				port_number = atoi(argv[cnt]);
			}
		}
		cnt++;
	}
}


int main(int argc, char* argv[])
{
	char buf[BUF_SIZE];
	struct sockaddr_in server_addr, client_addr;

	int server_sock;
	int len, msg_len;

	getopts(argc, argv);

	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		perror("error in socket()");
		return 1;
	}
	else
	{
		printf("socket is generated.\n");
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_number);

	if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
	{
		perror("error in bind()");
		return 1;
	}
	else
	{
		printf("socket bind is complete.\n");
	}

	if (listen(server_sock, 5) < 0)
	{
		perror("error in listen()");
		return 1;
	}
	else
	{
		printf("server socket is listening..\n");
		while(1);
	}

	return 0;
}

