#ifndef SSDP_SEARCH_MSG_H_
#define SSDP_SEARCH_MSG_H_

#include "ssdp.h"

#include <string>
using namespace std;

const string HOST 						= "HOST: 239.255.255.250:1900";
const string MAN 						= "MAN: \"ssdp:discover\"";
const string UDAP 						= "USER-AGENT: UDAP/2.0";

class SSDPSearchMsg {
private:
	int mMX;

public:
	SSDPSearchMsg();
	virtual ~SSDPSearchMsg();

	int getmMX();

	void setmMX(int m);

	string toString();
};

#endif//SSDP_SEARCH_MSG_H_
