#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int port_number = 8877; // default port number
char* msg = "hello!"; // default msg
char* ip = "127.0.0.1";

void getopts_with_params(int argc, char* argv[])
{
	int argc_cnt = 1;
	while (argc_cnt < argc)
	{
		if (!strcmp(argv[argc_cnt], "--port"))
		{
			if (++argc_cnt < argc)
			{
				port_number = atoi(argv[argc_cnt]);
			}
		}
		else if (!strcmp(argv[argc_cnt], "-m"))
		{
			if (++argc_cnt < argc)
			{
				msg = argv[argc_cnt];
			}
		}
		else if (!strcmp(argv[argc_cnt], "--ip"))
		{
			if (++argc_cnt < argc)
			{
				ip = argv[argc_cnt];
			}
		}
		argc_cnt++;
	}	
}


int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
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
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port_number);

	addr_len = sizeof(serv_addr);

	sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, addr_len);

	if ((recv_len = recvfrom(sock, recv_buf, 1024, 0, (struct sockaddr*)&serv_addr, &addr_len)) < 0)
	{
		perror("error in recvfrom()\n");
		return 1;
	}

	recv_buf[recv_len] = '\0';

	printf("ip: %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("received data: %s\n", recv_buf);

	close(sock);
}
