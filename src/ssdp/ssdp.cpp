#include "ssdp.h"

SSDP::SSDP() {

};

SSDP::~SSDP() {

}

ParsedDatagram *SSDP::convertDatagram(DatagramPacket *dp) {
	return new ParsedDatagram(dp);
}

