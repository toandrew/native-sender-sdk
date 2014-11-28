#ifndef DATAGRAM_PACKET_H_
#define DATAGRAM_PACKET_H_

#include "inet_socket_address.h"

#include <string>
using namespace std;

class DatagramPacket {
private:
	string data;
public:
	DatagramPacket(unsigned char *buf, int length);
	DatagramPacket(const char *buf, int length, InetSocketAddress *address);
	virtual ~DatagramPacket();

	string getData();
	void setData(unsigned char *buf, int length);
};

#endif //DATAGRAM_PACKET_H_
