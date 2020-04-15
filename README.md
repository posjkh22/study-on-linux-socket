# study-on-linux-socket-

# socket()
- File-based socket descriptor 를 생성 
- int socket(int domain, int type, int protocol);
- domain: AF_INET(IPv4), AF_INET6(IPv4)
- type: trasnport layer protocol - SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW(user-defined)
- protocol: 0 이면, type을 따름.


# TCP socket
server_sock = socket(AF_INET, SOCK_STREAM, 0);

# UDP socket 
server_sock = socket(AF_INET, SOCK_DGRAM, 0);


# IP address/ port number
- INADDR_ANY: "0.0.0.0" // 자신의 호스트에 들어오는 모든 패키을 수신, 실제 값은 0x00000000 으로 선언

# htonl(unsigned long )
server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0

# inet_iddr(const char* )
server_addr.sin_addr.s_addr = inet_iddr("127.0.0.1");


# IP address / port number example

sockaddr_in server_addr;
int port_number = 9988;
memset(&server_addr, 0x00, sizeof(server_addr));  //byte 단위 초기화
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
server_addr.sin_port = htons(port_number);

-> 0.0.0.0:9988 : port 9988로 오는 모든 패킷을 받음


# bind()
- socket에 ip address/port 설정.
- socket의 type (TCP/UDP)에 따라 동작이 상이.

# TCP socket bind
- socket에 ip address/port number 설정.

# UDP socket bind
- socket에 ip address/port number 설정.
- 설정한 port number로 port open.

# UDP socket bind & netstat (0.0.0.0:9988)
netsat -an
Proto / Local addr / foreign addr / state
UDP   0.0.0.0:9988    * : *      (empty)

# socket bind example
bind(server_sock, (struct sockaddr* )&server_addr, sizeof(server_addr);


# listen (TCP only)
- listen(int socketfd, int backlog)
- backlog: connect request가 대기할 수 있는 queue size
- client에서 connect request를 기다리고, socket를 listening 상태 만듬.

# socket listen example
listen(server_sock, 5);

# TCP socket listen & netstat (0.0.0.0:9988)
netsat -an
Proto / Local addr / foreign addr / state
TCP   0.0.0.0:9988    0.0.0.0:0  listening





