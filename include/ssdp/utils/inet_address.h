#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <string>

using namespace std;

class InetAddress {
public:
	InetAddress(const char *ip);
	virtual ~InetAddress();

	const char *getIpAddress();
private:
	const char* mIpAddress;
};

#endif // INET_ADDRESS_H_