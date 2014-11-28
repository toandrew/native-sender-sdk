#include "network_interface.h"

NetworkInterface::NetworkInterface() {

}

NetworkInterface::~NetworkInterface() {

}

NetworkInterface *NetworkInterface::getByInetAddress(InetAddress *address) {
	return new NetworkInterface();
}