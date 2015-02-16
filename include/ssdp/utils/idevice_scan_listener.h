#ifndef IDEVICE_SCAN_LISTENER_H_
#define IDEVICE_SCAN_LISTENER_H_

#include "flint_device.h"

class IDeviceScanListener {
public:
	IDeviceScanListener();
	virtual ~IDeviceScanListener();

public:
	void onAllDevicesOffline();

	void onDeviceOnline(FlintDevice *device);

	void onDeviceOffline(FlintDevice *device);

};

#endif //IDEVICE_SCAN_LISTENER_H_