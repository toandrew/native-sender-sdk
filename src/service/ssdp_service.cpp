#include "ssdp_service.h"

#include <string>
using namespace std;

SsdpService::SsdpService() {
	LOGI("SsdpService!");

	mDeviceScanner = new SsdpDeviceScanner();
}

SsdpService::~SsdpService() {
	LOGI("~SsdpService!");

	if (NULL != mDeviceScanner) {
		mDeviceScanner->stopScan();
		delete mDeviceScanner;
		mDeviceScanner = NULL;
	}
}

void SsdpService::start() {
	LOGI("SsdpService::start");

	if (mDeviceScanner != NULL) {
		mDeviceScanner->startScan();
	}
}

void SsdpService::stop() {
	LOGI("SsdpService::stop");

	if (mDeviceScanner != NULL) {
		mDeviceScanner->stopScan();
	}
}
