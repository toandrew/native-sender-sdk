#ifndef SSDP_H_
#define SSDP_H_

#include "parsed_datagram.h"
#include "datagram_packet.h"

#include <string>
#include <map>

using namespace std;

/* New line definition */
#define NEWLINE  						"\r\n"

#define ADDRESS 						"239.255.255.250"
#define PORT 							1900
#define SOURCE_PORT 					1901

#define ST 								"ST"
#define LOCATION 						"LOCATION"
#define NT 								"NT"
#define NTS 							"NTS"

#define URN 							"URN"
#define USN 							"USN"

#define APPLICATION_URL 				"Application-URL"

/* Definitions of start line */
#define SL_NOTIFY 						"NOTIFY * HTTP/1.1"
#define SL_MSEARCH 						"M-SEARCH * HTTP/1.1"
#define SL_OK 							"HTTP/1.1 200 OK"

#define NTS_ALIVE 						"ssdp:alive"
#define NTS_BYEBYE 						"ssdp:byebye"
#define NTS_UPDATE 						"ssdp:update"

/* Definitions of search targets */
#define ST_SSAP 				    	"ST: urn:lge-com:service:webos-second-screen:1"
#define ST_DIAL 						"ST: urn:dial-multiscreen-org:service:dial:1"
#define DEVICE_MEDIA_SERVER_1 			"urn:schemas-upnp-org:device:MediaServer:1"

#ifndef TYPE_BYTE
#define TYPE_BYTE
typedef unsigned char byte;
#endif

class SSDP {
public:
	static ParsedDatagram *convertDatagram(DatagramPacket *dp);

	SSDP();
	virtual ~SSDP();
};

#endif // SSDP_H_