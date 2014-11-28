#include "ssdp_socket.h"

#include <string>
using namespace std;

SSDPSocket::SSDPSocket(InetAddress *address) : mTimeout(0) {
    mInetAddress = address;

	mSSDPMulticastGroup = new InetSocketAddress(ADDRESS, PORT);

	mMuticastSocket = new MulticastSocket(PORT);

	mNetIf = NetworkInterface::getByInetAddress(mInetAddress);

	mMuticastSocket->joinGroup(mSSDPMulticastGroup, mNetIf);

    mWildSocket = new DatagramSocket();
    mWildSocket->setReuseAddress(true);
    mWildSocket->bind(new InetSocketAddress(mInetAddress, SOURCE_PORT));
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
}

DatagramPacket *SSDPSocket::responseReceive() {
	byte* buf = new byte[1024];
    DatagramPacket *dp = new DatagramPacket(buf, 1024);

    mWildSocket->receive(dp);

    return dp;
}

DatagramPacket *SSDPSocket::notifyReceive() {
	byte* buf = new byte[1024];
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
