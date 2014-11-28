#include "datagram_socket.h"

#define SO_REUSEPORT 15 // need  change it

DatagramSocket::DatagramSocket() {
	mIsConnected = false;
	mIsClosed = false;

	mSocket = -1;
}

DatagramSocket::~DatagramSocket() {
	close();
}

void DatagramSocket::setReuseAddress(bool reuse) {

}

void DatagramSocket::bind(InetSocketAddress *address) {
	int ret = 0;

    // Bind to all interface(s)
    ret = ::bind(mSocket, (struct sockaddr*)&mSrcaddr, sizeof(struct sockaddr));
    if (ret < 0 ) {
    	LOGE("address in use!");

    	::close(mSocket);
    	return;
    }
}

void DatagramSocket::send(DatagramPacket *dp) {
	if (dp == NULL) {
		LOGE("DatagramSocket send: data is NULL!");
		return;
	}

	if (mSocket < 0) {
		createSocket();
	}

	string str = dp->getData();
	/*
    str.seekp(0, ios::end);
    stringstream::pos_type length = str.tellp();
    str.seekp(0, ios::beg);
	*/
	if(mSocket != -1) {
        sendto(mSocket, str.c_str(), str.length(), 0, (struct sockaddr*)&mSrcaddr , sizeof(struct sockaddr));
	} else {
        LOGE("invalid socket!");
    }
}

void DatagramSocket::receive(DatagramPacket *dp) {
	if (mSocket < 0) {
		createSocket();
	}

	int ret = 0;
    unsigned char buf[4096] = {0};
    int bufsize = 4096;
    struct sockaddr_in sender;
    socklen_t senderlen = sizeof(struct sockaddr);

    ret = recvfrom(mSocket, buf, bufsize, 0, (struct sockaddr*)&sender, &senderlen);
    if(ret != -1){
    	dp->setData(buf, ret);
    }

    LOGE("RECEIVED!!!!");
}

bool DatagramSocket::isConnected() {
	return mIsConnected;
}

void DatagramSocket::close() {
	if (mSocket >= 0) {
		::close(mSocket);
	}

	mIsClosed = true;
}

void DatagramSocket::disconnect() {
}

void DatagramSocket::setSoTimeout(int timeout) {
}

int DatagramSocket::createSocket() {
	int ret = 0;
	int optval = 0;

	mSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (mSocket < 0) {
		LOGE("cannot create socket!");
		return -1;
	}

	// Set nonblocking
	optval = fcntl(mSocket, F_GETFL, 0);
	if (optval < 0) {
		LOGE("cannot fcntl[F_GETFL] socket!");
		::close(mSocket);

		mSocket = -1;
		return -1;
	}
	ret = fcntl(mSocket, F_SETFL, optval | O_NONBLOCK);
	if (ret < 0) {
		LOGE("cannot fcntl[F_SETFL] socket!");
		::close(mSocket);

		mSocket = -1;
		return -1;
	}

    //Reuse port
    optval = 1;
    ret = setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, 4);
    if (ret < 0) {
    	LOGE("cannot setsockopt[SO_REUSEADDR] socket!");
		::close(mSocket);

		mSocket = -1;
		return -1;
    }
    optval = 1;
    ret = setsockopt(mSocket, SOL_SOCKET, SO_REUSEPORT, (char*)&optval, 4);
    if (ret < 0) {
    	LOGE("cannot setsockopt[SO_REUSEPORT] socket!");
		::close(mSocket);

		mSocket = -1;
		return -1;
    }

    return mSocket;
}