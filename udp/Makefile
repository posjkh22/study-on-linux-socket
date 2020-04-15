
all: udp_client udp_server

udp_client: udp_client.c
	gcc -o udp_client udp_client.c

udp_server: udp_server.c
	gcc -o udp_server udp_server.c

clean:
	rm -f *.o udp_client udp_server
