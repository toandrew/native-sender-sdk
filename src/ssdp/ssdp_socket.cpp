#include "ssdp_socket.h"

#include <string>
using namespace std;

SSDPSocket::SSDPSocket(InetAddress *address) : mTimeout(0) {
    mInetAddress = address;

	mSSDPMulticastGroup = new InetSocketAddress(ADDRESS, PORT);

    LOGE("CREATE Multicast SOCKET!");
	mMuticastSocket = new MulticastSocket(PORT);

	mNetIf = NetworkInterface::getByInetAddress(mInetAddress);

	mMuticastSocket->joinGroup(mSSDPMulticastGroup, mNetIf);

    LOGE("CREATE Wild SOCKET!");
    mWildSocket = new DatagramSocket();
    mWildSocket->setReuseAddress(true);
    mWildSocket->bind(new InetSocketAddress("0.0.0.0", PORT));
}

SSDPSocket::~SSDPSocket() {
	if (mMuticastSocket != NULL) {
        mMuticastSocket->leaveGroup(mSSDPMulticastGroup, mNetIf);

		delete mMuticastSocket;
		mMuticastSocket = NULL;
	}

    if (mSSDPMulticastGroup != NULL) {
        delete mSSDPMulticastGroup;

        mSSDPMulticastGroup = NULL;
    }

}

void SSDPSocket::send(string data) {
	DatagramPacket *dp = new DatagramPacket(data.c_str(), data.length(),
                mSSDPMulticastGroup);

    mWildSocket->send(dp);

    delete dp;
    dp = NULL;
}

DatagramPacket *SSDPSocket::responseReceive() {
	char* buf = new char[1024];
    DatagramPacket *dp = new DatagramPacket(buf, 1024);

    mWildSocket->receive(dp);

    return dp;
}

DatagramPacket *SSDPSocket::notifyReceive() {
	char* buf = new char[1024];
    DatagramPacket *dp = new DatagramPacket(buf, 1024);

    mMuticastSocket->receive(dp);

    return dp;
}

bool SSDPSocket::isConnected() {
	return mWildSocket != NULL && mMuticastSocket != NULL 
			&& mWildSocket->isConnected() && mMuticastSocket->isConnected();
}

void SSDPSocket::close() {
    if (mMuticastSocket != NULL) {
        mMuticastSocket->leaveGroup(mSSDPMulticastGroup, mNetIf);

        mMuticastSocket->close();
    }

    if (mWildSocket != NULL) {
        mWildSocket->disconnect();
        mWildSocket->close();
    }
}

void SSDPSocket::setTimeout(int timeout) {
    mTimeout = timeout;

    if (mWildSocket != NULL) {
       	mWildSocket->setSoTimeout(mTimeout);
    }
}
