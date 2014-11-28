#ifndef NETWORK_INTERFACE_H_
#define NETWORK_INTERFACE_H_

#include "inet_address.h"

class NetworkInterface {
public:
	NetworkInterface();
	virtual ~NetworkInterface();

	static NetworkInterface *getByInetAddress(InetAddress *addres);
};

#endif // NETWORK_INTERFACE_H_