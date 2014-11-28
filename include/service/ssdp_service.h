#ifndef SSDP_SERVICE_H_
#define SSDP_SERVICE_H_

#include "ssdp_device_scanner.h"
#include "logger.h"

#include <string>
using namespace std;

class SsdpService {
private:
	DeviceScanner *mDeviceScanner;

public:
	SsdpService();
	virtual ~SsdpService();

	void start();
	void stop();
};

#endif // SSDP_SERVICE_H_