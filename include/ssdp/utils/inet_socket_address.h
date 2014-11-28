#ifndef INET_SOCKET_ADDRESS_H_
#define INET_SOCKET_ADDRESS_H_

#include "inet_address.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

class InetSocketAddress {
private:
	InetAddress *mAddress;
	int mPort;
	char mIpAddress[256];
public:
	InetSocketAddress(const char *host, int port);
	InetSocketAddress(InetAddress *address, int port);
	virtual ~InetSocketAddress();

	InetAddress *getAddress();
	char *getIpAddress();
	int getPort();
};

#endif // INET_SOCKET_ADDRESS_H_
