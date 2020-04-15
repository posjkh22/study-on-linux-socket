
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUF_SIZE 128

int port_number = 1663;

void getopts(int argc, char* argv[])
{
	int cnt = 1;
	while (cnt < argc)
	{
		if (!strcmp(argv[cnt], "--port"))
		{
			if (++cnt<argc)
			{
				port_number = atoi(argv[cnt]);
			}
		}
		cnt++;
	}
}

int main(int argc, char* argv[])
{
	int server_socket;
	int len;
	struct sockaddr_in server_addr;
	char buf[BUF_SIZE];

	getopts(argc, argv);

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error in socket()");
		return 1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(port_number);

	printf("port number: %d\n", port_number);

	if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		perror("error in connect()");
		return 1;
	}
	else
	{
		printf("connection with server is complete\n");
		while(1);
	}
	
	while(1)
	{	
		printf("msg to server: "); 
		scanf("%s", buf);
		fflush(stdin);
		if ((len = write(server_socket, buf, BUF_SIZE)) < 0)
		{
			printf("- sent to server\n");
			memset(buf, 0x00, sizeof(buf));
			if ((len = read(server_socket, buf, BUF_SIZE)) > 0)
			{
				printf("- received from server: ");
				buf[len] = NULL;
				printf("%s", buf);
		
			}
		}
		memset(buf, 0x00, sizeof(buf));
	}
	close(server_socket);
}
