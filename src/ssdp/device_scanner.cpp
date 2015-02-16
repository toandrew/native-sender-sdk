#include "device_scanner.h"
#include "logger.h"

#include <string>
using namespace std;

DeviceScanner::DeviceScanner() {
	LOGI("DeviceScanner");
}

DeviceScanner::~DeviceScanner() {
	LOGI("~DeviceScanner");
}

void DeviceScanner::startScan() {
	LOGI("startScan");
	startScanInternal(NULL);
}

void DeviceScanner::stopScan() {
	LOGI("stopScan");
	stopScanInternal();
}

void DeviceScanner::addListener(IDeviceScanListener *listener) {

}

void DeviceScanner::notifyDeviceOffline(FlintDevice *device) {

}

list<IDeviceScanListener> *DeviceScanner::getDeviceScannerListenerList() {
	return &mListenerList;
}

void DeviceScanner::reportNetworkError() {

}

void DeviceScanner::reportError() {

}

// private methods
void DeviceScanner::startScanInit() {

}

void DeviceScanner::stopScanInit() {

}
