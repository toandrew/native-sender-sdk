#ifndef PARSED_DATAGRAM_H_
#define PARSED_DATAGRAM_H_

#include "datagram_packet.h"

#include <string>
#include <map>
using namespace std;

class ParsedDatagram {
public:
	DatagramPacket *dp;
	map<string, string> *data;
	string type;
public:
	ParsedDatagram(DatagramPacket *packet);
	virtual ~ParsedDatagram();

	static string asciiUpper(string text);
};

#endif //PARSED_DATAGRAM_H_