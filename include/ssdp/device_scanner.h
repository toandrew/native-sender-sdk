#ifndef DEVICE_SCANNER_H_
#define DEVICE_SCANNER_H_

#include "fling_device.h"
#include "network_interface.h"
#include "idevice_scan_listener.h"

#include <string>
#include <list>
using namespace std;

class DeviceScanner {
private:
	void startScanInit();

	void stopScanInit();

public:
	DeviceScanner();
	virtual ~DeviceScanner();

	void startScan();

	void stopScan();

	void addListener(IDeviceScanListener *listener);

	void notifyDeviceOffline(FlingDevice *device);

	list<IDeviceScanListener> *getDeviceScannerListenerList();

	void reportNetworkError();

	void reportError();

	virtual void setDeviceOffline(string deviceId) = 0;

	virtual void onAllDevicesOffline() = 0;

	virtual void startScanInternal(list<NetworkInterface> *networks) = 0;

	virtual void stopScanInternal() = 0;
};

#endif //DEVICE_SCANNER_H_