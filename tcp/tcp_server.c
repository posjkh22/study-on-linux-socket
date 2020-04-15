
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 128

int port_number = 1664;

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
	char client_nice_addr[20];

	int server_sock, client_sock;
	int len, msg_len;

	getopts(argc, argv);


	if ( (server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("error in socket()");
		return 1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(port_number);

	if ( bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
	{
		perror("error in bind()");
		return 1;
	}
	

	if ( listen(server_sock, 5) < 0)
	{
		perror("error in listen()");
		return 1;
	}

	memset(buf, 0x00, sizeof(buf));
	printf("waiting connection request..\n");
	
	while(1);


	len = sizeof(client_addr);
	while(1)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);

		if (client_sock < 0)
		{
			perror("error in aceept()");
			return 1;
		}


		if (!inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_nice_addr, sizeof(client_nice_addr)))
		{
			perror("error in inet_ntop()");
			return 1;
		}
		printf("client(%s) is connected.\n", client_nice_addr);

		while(1)
		{
			msg_len = read(client_sock, buf, 1024);
			if (msg_len == 0)
			{
				break;
			}
			buf[msg_len] = '\0';
			printf("msg from client: %s\n", buf);

			write(client_sock, buf, msg_len);
		}
		close(client_sock);
		printf("client(%s) is closed.\n", client_nice_addr);
	}
	close(server_sock);
	return 0;
}
