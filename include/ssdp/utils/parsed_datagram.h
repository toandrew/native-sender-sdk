#ifndef PARSED_DATAGRAM_H_
#define PARSED_DATAGRAM_H_

#include "logger.h"

#include "datagram_packet.h"

#include <string>
#include <map>
using namespace std;

class ParsedDatagram {
public:
	DatagramPacket *dp;
	map<string, string> data;
	string type;
public:
	ParsedDatagram(DatagramPacket *packet);
	virtual ~ParsedDatagram();

	static string asciiUpper(string text);

	map<string, string> getData();

	string getType();

private:
	int fillMaps(const string& s, const string& delim);
};

#endif //PARSED_DATAGRAM_H_