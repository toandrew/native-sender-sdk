#include "multicast_socket.h"

MulticastSocket::MulticastSocket(int port): mPort(port) {
}

MulticastSocket::~MulticastSocket() {
}

void MulticastSocket::joinGroup(InetSocketAddress *address, NetworkInterface *netIf) {
	int ret = 0;

	if (mSocket < 0) {
		create();
	}

	if (mSocket < 0) {
		LOGE("FAILED TO JOINGROUP!");

		return;
	} 

/*
    int optval = 0;
    ret = setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&optval, sizeof(int));
    if (ret < 0) {
      LOGE("FAILED TO IP_ADD_MEMBERSHIP!");

      ::close(mSocket);
      return;
    }

*/

    // Add membership
    mMreq.imr_multiaddr.s_addr = inet_addr(address->getIpAddress());
    mMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    ret = setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mMreq, sizeof(struct ip_mreq));
   	if (ret < 0) {
   		LOGE("FAILED TO IP_ADD_MEMBERSHIP!");

   		::close(mSocket);
   		return;
   	}

    ///InetSocketAddress *ad = new InetSocketAddress("0.0.0.0", 0); 

    LOGE("MulticastSocket::joinGroup:READY TO BIND?!");
   	//bind(address);

    //delete ad;
    //ad = NULL;
}

void MulticastSocket::leaveGroup(InetSocketAddress *address, NetworkInterface *netIf) {
	int ret = 0;
	
	// leave group!
    ret = setsockopt(mSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mMreq, sizeof(struct ip_mreq));
   	if (ret < 0) {
   		LOGE("FAILED TO IP_ADD_MEMBERSHIP!");

   		::close(mSocket);
   		return;
   	}
}

void MulticastSocket::create() {
	int optval = 0;
	int ret = 0;

	int socket = createSocket();
	if (socket < 0) {
		LOGE("FAILED TO CREATE MULTICAST SOCKET!");
		return;
	}

	// Source address
    mSrcaddr.sin_family = PF_INET;
    mSrcaddr.sin_port = htons(mPort); //Let the IP stack decide
    mSrcaddr.sin_addr.s_addr = INADDR_ANY; //Default multicast nic??inet_addr(ip)?

    // Disable loopback
    optval = 0;
    ret = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&optval, sizeof(int));
    if (ret < 0) {
    	LOGE("Failed to setsockopt[IP_MULTICAST_LOOP]!");

    	::close(socket);
    	return;
    }

    // TTL
    optval = 2;
    ret = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&optval, sizeof(int));
    if (ret < 0) {
    	LOGE("Failed to setsockopt[IP_MULTICAST_TTL]!");

    	::close(socket);
    	return;
    }

    // buffer size
    optval = 5000;
    ret = setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(int));
    if (ret < 0) {
    	LOGE("Failed to setsockopt[SO_SNDBUF]!");

    	::close(socket);
    	return;
    }
}
