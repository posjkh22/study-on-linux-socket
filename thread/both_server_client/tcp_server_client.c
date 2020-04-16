
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 128

void* init_tcp_server(void* data);
void* init_tcp_client(void* data);


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

	pthread_t thread_server, thread_client;

	getopts(argc, argv);

	if (pthread_create(&thread_server, NULL, init_tcp_server, NULL) < 0)
	{
		perror("error in pthread_create (server)");
		return 1;
	}
	printf("- server thread has started!\n");

	if (pthread_create(&thread_client, NULL, init_tcp_client, NULL) < 0)
	{
		perror("error in pthread_create (client)");
		return 1;
	}
	printf("- client thread has started!\n");

	pthread_join(thread_server, NULL);
	pthread_join(thread_client, NULL);

}


void* init_tcp_server(void* data)
{

	char buf[BUF_SIZE];
	struct sockaddr_in server_addr, client_addr;
	char client_nice_addr[20];

	int server_sock, client_sock;
	int len, msg_len;


	if ( (server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("[server] error in socket()");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(port_number);

	if ( bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
	{
		perror("[server] error in bind()");
		exit(0);
	}
	

	if ( listen(server_sock, 5) < 0)
	{
		perror("[server] error in listen()");
		exit(0);
	}

	memset(buf, 0x00, sizeof(buf));
	printf("[server] waiting connection request..\n");
	
	len = sizeof(client_addr);
	while(1)
	{
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);

		if (client_sock < 0)
		{
			perror("[server] error in aceept()");
			exit(0);
		}


		if (!inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_nice_addr, sizeof(client_nice_addr)))
		{
			perror("[server] error in inet_ntop()");
			exit(0);
		}
		printf("[server] client(%s) is connected.\n", client_nice_addr);

		while(1)
		{
			msg_len = read(client_sock, buf, 1024);
			if (msg_len == 0)
			{
				break;
			}
			buf[msg_len] = '\0';
			printf("[server] msg from client: %s\n", buf);

			write(client_sock, buf, msg_len);
		}
		close(client_sock);
		printf("[server] client(%s) is closed.\n", client_nice_addr);
	}
	close(server_sock);

}

void* init_tcp_client(void* data)
{
	int server_socket;
	int len;
	struct sockaddr_in server_addr;
	char buf[BUF_SIZE];

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("[client] error in socket()");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(port_number);

	printf("[client] port number: %d\n", port_number);

	if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		perror("[client] error in connect()");
		exit(0);
	}
	else
	{
		printf("[client] connection with server is complete\n");
	}
	
	while(1)
	{	
		printf("[client] msg to server: "); 
		scanf("%s", buf);
		fflush(stdin);
		if ((len = write(server_socket, buf, BUF_SIZE)) < 0)
		{
			printf("[client] - sent to server\n");
			memset(buf, 0x00, sizeof(buf));
			if ((len = read(server_socket, buf, BUF_SIZE)) > 0)
			{
				printf("[client] - received from server: ");
				buf[len] = NULL;
				printf("%s", buf);
		
			}
		}
		memset(buf, 0x00, sizeof(buf));
	}
	close(server_socket);

}

