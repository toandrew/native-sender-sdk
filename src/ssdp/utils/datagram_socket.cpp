#include "datagram_socket.h"
#include <regex.h>
#include <vector>

#define SO_REUSEPORT 15 // need  change it

#define SSDP_RESPONSE_HEADER "^HTTP\/[0-9]\.[0-9] [0-9][0-9][0-9] OK$"


static void split(const string& s, const string& delim, vector<string> &ret) {
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos) {
		ret.push_back(s.substr(last, index-last));
		last = index + 1; 
		index = s.find_first_of(delim, last);
	}

	if (index-last > 0) {
		ret.push_back(s.substr(last, index - last));
	}
}

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

	mSrcaddr.sin_family = PF_INET;
	mSrcaddr.sin_addr.s_addr = inet_addr(address->getIpAddress());
	mSrcaddr.sin_port = htons(address->getPort());

    printf("DatagramSocket::bind![%s][%d]\n", address->getIpAddress(), address->getPort());
    // Bind to all interface(s)
    ret = ::bind(mSocket, (struct sockaddr*)&mSrcaddr, sizeof(struct sockaddr));
    if (ret < 0 ) {
    	LOGE("sorry!address in use!close socket!");

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

	InetSocketAddress *address = dp->getAddress();
	mDstaddr.sin_family = PF_INET;
	mDstaddr.sin_addr.s_addr = inet_addr(address->getIpAddress());
	mDstaddr.sin_port = htons(address->getPort());

	string str = dp->getData();
	/*
    str.seekp(0, ios::end);
    stringstream::pos_type length = str.tellp();
    str.seekp(0, ios::beg);
	*/
	if(mSocket != -1) {
		LOGE("DatagramSocket send: send it!");
        ::sendto(mSocket, str.c_str(), str.length(), 0, (struct sockaddr*)&mDstaddr , sizeof(struct sockaddr));
	} else {
        LOGE("invalid socket!");
    }
}

void DatagramSocket::receive(DatagramPacket *dp) {
	if (mSocket < 0) {
		createSocket();
	}

	int ret = 0;
    char buf[4096] = {0};
    int bufsize = 4096;
    struct sockaddr_in sender;
    socklen_t senderlen = sizeof(struct sockaddr);

    ret = recvfrom(mSocket, buf, bufsize, 0, (struct sockaddr*)&sender, &senderlen);
    if(ret != -1){
    	dp->setData(buf, ret);

    	buf[ret] = '\0';
    }

	vector<string> vt;
	string delim("\r\n");
	split(dp->getData(), delim, vt);

    printf("RECEIVED!!!![%d][%d][%s][%s]\n", ret, vt.size(), vt[0].c_str(), buf);

     int nErrCode = 0;
     regex_t oRegex;
     if ((nErrCode = regcomp(&oRegex, SSDP_RESPONSE_HEADER, 0)) == 0) {
     	if ((nErrCode = regexec(&oRegex, vt[0].c_str(), 0, NULL, 0)) == 0) {
     		printf("%s matches %s\n", SSDP_RESPONSE_HEADER, vt[0].c_str());
     		regfree(&oRegex);
     		return;
     	}
     }

     char szErrMsg[1024] = {0};
     size_t unErrMsgLen = 0;
     unErrMsgLen = regerror(nErrCode, &oRegex, szErrMsg, sizeof(szErrMsg));
     unErrMsgLen = unErrMsgLen < sizeof(szErrMsg) ? unErrMsgLen : sizeof(szErrMsg) - 1;
     szErrMsg[unErrMsgLen] = '\0';
     printf("ErrMsg: %s\n", szErrMsg);
     regfree(&oRegex);
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

	/*
	ret = fcntl(mSocket, F_SETFL, optval | O_NONBLOCK);
	if (ret < 0) {
		LOGE("cannot fcntl[F_SETFL] socket!");
		::close(mSocket);

		mSocket = -1;
		return -1;
	}
	*/

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
