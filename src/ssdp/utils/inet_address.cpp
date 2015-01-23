#include "inet_address.h"

InetAddress::InetAddress(const char *ip) {
  mIpAddress = ip;
}

InetAddress::~InetAddress() {
	
}

const char *InetAddress::getIpAddress() {
	return mIpAddress;
}