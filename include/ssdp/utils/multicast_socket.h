#ifndef MULTICAST_SOCKET_H_
#define MULTICAST_SOCKET_H_

#include "inet_socket_address.h"
#include "network_interface.h"
#include "datagram_packet.h"
#include "datagram_socket.h"

class MulticastSocket : public DatagramSocket {
private:
	int mPort;

	struct ip_mreq mMreq;
public:
	MulticastSocket(int port);
	virtual ~MulticastSocket();

	void joinGroup(InetSocketAddress *address, NetworkInterface *netIf);

	void leaveGroup(InetSocketAddress *address, NetworkInterface *netIf);
private:
	void create();
};

#endif // MULTICAST_SOCKET_H_