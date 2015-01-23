#include "datagram_packet.h"

DatagramPacket::DatagramPacket(const char *buf, int length) : data(buf, length) {
}

DatagramPacket::DatagramPacket(const char *buf, int length, InetSocketAddress* address) : data(buf, length) {
	mAddress = address;
}

DatagramPacket::~DatagramPacket() {

}

string DatagramPacket::getData() {
	return data;
}

void DatagramPacket::setData(char *buf, int length) {
	string a(buf,length);
	data = a+'\n';
}

InetSocketAddress *DatagramPacket::getAddress() {
	return mAddress;
}