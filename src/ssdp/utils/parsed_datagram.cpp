#include "parsed_datagram.h"

ParsedDatagram::ParsedDatagram(DatagramPacket *packet) {
	dp = packet;
};

ParsedDatagram::~ParsedDatagram() {
}

string ParsedDatagram::asciiUpper(string text) {
	return text;
}
