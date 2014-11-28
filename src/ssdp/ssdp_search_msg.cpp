#include "ssdp_search_msg.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

SSDPSearchMsg::SSDPSearchMsg() : mMX(5) {

}

SSDPSearchMsg::~SSDPSearchMsg() {

}

int SSDPSearchMsg::getmMX() {
	return this->mMX;
}

void SSDPSearchMsg::setmMX(int m) {
	this->mMX = m;
}

string SSDPSearchMsg::toString() {
	ostringstream ss;
	ss << SL_MSEARCH << NEWLINE
	   << HOST << NEWLINE
	   << MAN << NEWLINE
	   << ST_DIAL << NEWLINE
	   << "MX: " << mMX << NEWLINE
	   << NEWLINE;

	string content = ss.str();
	
	return content;
}