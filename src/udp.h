#ifndef __UDP__
#define __UDP__

#define UDP_RECV_PORT ((uint16_t) 8888)
#define UDP_SRC_PORT ((uint16_t) 1234)
#define UDP_MULTICAST_ADDRESS ("ff02::1")

typedef struct packet{
	int header;
	int sequence_number;
	int payload;
} TPacket;

int udp_send(TPacket* p)
int udp_server_start(void)

#endif
