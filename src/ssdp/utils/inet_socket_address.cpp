#include "inet_socket_address.h"

InetSocketAddress::InetSocketAddress(const char *host, int port) {
	sprintf(mIpAddress,"%s", host);

	mPort = port;
}

InetSocketAddress::InetSocketAddress(InetAddress *address, int port) {
	sprintf(mIpAddress,"%s", address->getIpAddress());

	mPort = port;
}

InetSocketAddress::~InetSocketAddress() {

}

InetAddress *InetSocketAddress::getAddress() {
	return mAddress;
}

char *InetSocketAddress::getIpAddress() {
	return mIpAddress;
}

int InetSocketAddress::getPort() {
	return mPort;
}