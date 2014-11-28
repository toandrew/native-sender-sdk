#include "datagram_packet.h"

DatagramPacket::DatagramPacket(unsigned char *buf, int length) {

}

DatagramPacket::DatagramPacket(const char *buf, int length, InetSocketAddress* address) {

}

DatagramPacket::~DatagramPacket() {

}

string DatagramPacket::getData() {
	return data;
}

void DatagramPacket::setData(unsigned char *buf, int length) {
	//data = buf;
}