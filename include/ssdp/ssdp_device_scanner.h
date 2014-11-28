#ifndef SSDP_DEVICE_SCANNER_H_
#define SSDP_DEVICE_SCANNER_H_

#include "device_scanner.h"
#include "ssdp.h"
#include "ssdp_socket.h"
#include "ssdp_search_msg.h"
#include "timer.h"
#include "inet_address.h"
#include "runnable.h"
#include "thread.h"
#include "pattern.h"

#include "logger.h"

#include <string>
#include <list>
#include <map>
//#include <regex>

using namespace std;

class ScannerPrivData;

class LocationDevice;

class LocationImage;

class SsdpDeviceScanner: public DeviceScanner {
private:
	const static int NUM_OF_THREADS 		= 20;
	const static int RESCAN_INTERVAL 		= 10; //10s

	map<string, ScannerPrivData> *mScannerData;

	SSDPSocket *mSSDPSocket;
	SSDPSearchMsg *mSSDPSearchMsg;

	Thread *mResponseThread;
	Thread *mNotifyThread;

	Pattern *uuidReg;

	list<string> *mDiscoveredDeviceList;

	map<string, string> *mFoundDeviceMap;

	Timer *mCheckOfflineTimer;

	Timer *mSendSearchTimer;

	bool mStarting;

	Runnable *mResponseHandler;

	Runnable *mRespNotifyHandler;

public:
	SsdpDeviceScanner();
	virtual ~SsdpDeviceScanner();

	virtual void setDeviceOffline(string id);
	virtual void onAllDevicesOffline();

protected:
	virtual void startScanInternal(list<NetworkInterface> *networks);
	virtual void stopScanInternal();

private:
	void start();

	void stop();

	void openSocket();

	InetAddress *getIpAddress();

	char *convertIpAddress(int ip);

	void handleDatagramPacket(ParsedDatagram *pd);

	void getLocationData(string location, string uuid);

	void onResult(string uuid, LocationDevice *device);

	static void *checkDeviceOffline(void *data);

	static void *sendSsdpSearchRequest(void *data);

	static void* handleResponse(void *data);
	static void* handleResponseNotify(void *data);
};


class ScannerPrivData {
	FlingDevice *mFlingDevice;
    long mElapsedRealtime;
    long mTTl;
    bool d;
    string mUuid;
};

class LocationDevice {
	list<LocationImage> *iconImages;
    string url;
    string friendlyName;
    string manufacturer;
    string modelName;
};

class LocationImage {
	string mimetype;
    string width;
    string height;
    string depth;
    string url;
};

#endif // SSDP_DEVICE_SCANNER_H_
