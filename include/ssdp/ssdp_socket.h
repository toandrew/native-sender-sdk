#ifndef SSDP_SOCKET_H_
#define SSDP_SOCKET_H_

#include "ssdp.h"
#include "socket_address.h"
#include "datagram_socket.h"
#include "multicast_socket.h"
#include "network_interface.h"
#include "inet_address.h"
#include "inet_socket_address.h"

#include <string>
using namespace std;

#define SOCKET 						int
#define INVALID_SOCKET 				-1
#define SOCKET_ERROR 				-1

class SSDPSocket {
private:
	int mTimeout;

	InetSocketAddress *mSSDPMulticastGroup;
	DatagramSocket *mWildSocket;
	MulticastSocket *mMuticastSocket;
	NetworkInterface *mNetIf;
	InetAddress *mInetAddress;

public:
	SSDPSocket(InetAddress *address);
	virtual ~SSDPSocket();

	void send(string data);

	DatagramPacket *responseReceive();

	DatagramPacket *notifyReceive();

	bool isConnected();

	void close();

	void setTimeout(int timeout);
};

#endif //SSDP_SOCKET_H_
