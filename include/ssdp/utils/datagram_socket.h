#ifndef DATAGRAM_SOCKET_H
#define DATAGRAM_SOCKET_H

#include "inet_socket_address.h"
#include "datagram_packet.h"
#include "logger.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string>
using namespace std;

class DatagramSocket {
private:
	InetSocketAddress *mBindAddress;

	bool mIsConnected;
	bool mIsClosed;

protected:
	int mSocket;
	
	struct sockaddr_in mSrcaddr;

public:
	DatagramSocket();
	virtual ~DatagramSocket();

	void setReuseAddress(bool reuse);

	void bind(InetSocketAddress *address);

	void send(DatagramPacket *dp);

	void receive(DatagramPacket *dp);

	bool isConnected();

	void close();

	void disconnect();

	void setSoTimeout(int timeout);

protected:
	int createSocket();
};


#endif // DATAGRAM_SOCKET_H
