#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int port_number = 8889;

void getopts_with_params(int argc, char* argv[])
{
	int arg_cnt = 1;
	while (arg_cnt < argc)
	{
		if (!strcmp(argv[arg_cnt], "--port"))
		{
			port_number = atoi(argv[++arg_cnt]);
		}
		arg_cnt++;
	}
}

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr, client_addr;
	char recv_buf[1024];
	int recv_len;
	int addr_len;

	getopts_with_params(argc, argv);


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error in socket()\n");
		return 1;
	}		

	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_number);

	printf("server port number: %d\n", port_number);
	

	if ((bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
	{
		perror("error in bind()\n");
		return 1;
	}

	printf("waiting for messages..\n");


	addr_len = sizeof(client_addr);
	if ((recv_len = recvfrom(sock, recv_buf, 1024, 0, (struct sockaddr *)&client_addr, &addr_len)) < 0)
	{
		perror("error in recvfrom()\n");
		return 1;
	}

	recv_buf[recv_len] = '\0';

	printf("ip: %s\n", inet_ntoa(client_addr.sin_addr));
	printf("received data: %s\n", recv_buf);

	sendto(sock, recv_buf, strlen(recv_buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	
	close(sock);	
}
