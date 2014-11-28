#include "ssdp_device_scanner.h"

SsdpDeviceScanner::SsdpDeviceScanner() :mStarting(false) {
	LOGI("SsdpDeviceScanner!");
}

SsdpDeviceScanner::~SsdpDeviceScanner() {
	LOGI("~SsdpDeviceScanner!");

	// stop all!
	stop();
}

void SsdpDeviceScanner::startScanInternal(list<NetworkInterface> *networks) {
	LOGI("SsdpDeviceScanner::startScanInternal!");

	start();
}

void SsdpDeviceScanner::stopScanInternal() {
	LOGI("SsdpDeviceScanner::stopScanInternal!");

	stop();
}

void SsdpDeviceScanner::setDeviceOffline(string id) {
}

void SsdpDeviceScanner::onAllDevicesOffline() {
}

// private methods
void SsdpDeviceScanner::start() {
	LOGI("SsdpDeviceScanner::start!");

	if (mStarting) {
		return;
	}

	mStarting = true;

	stop();

	openSocket();

	mSSDPSearchMsg = new SSDPSearchMsg();

	mCheckOfflineTimer = new Timer();
	mCheckOfflineTimer->schedule(checkDeviceOffline, 1, SsdpDeviceScanner::RESCAN_INTERVAL, (void *)this);

	mSendSearchTimer = new Timer();
	mSendSearchTimer->schedule(sendSsdpSearchRequest, 1, SsdpDeviceScanner::RESCAN_INTERVAL, (void *)this);

	mResponseHandler = new Runnable(handleResponse);
	mResponseThread = new Thread(mResponseHandler);

    mRespNotifyHandler = new Runnable(handleResponseNotify);
	mNotifyThread = new Thread(mRespNotifyHandler);

	mResponseThread->start();
	mNotifyThread->start();
}

void SsdpDeviceScanner::stop() {
	LOGI("SsdpDeviceScanner::stop!");

	mStarting = false;

	if (mCheckOfflineTimer != NULL) {
		mCheckOfflineTimer->cancel();

		delete mCheckOfflineTimer;
		mCheckOfflineTimer = NULL;
	}
        
	if (mSendSearchTimer != NULL) {
		mSendSearchTimer->cancel();

		delete mSendSearchTimer;
		mSendSearchTimer = NULL;
	}
        
	if (mResponseThread != NULL) {
    	mResponseThread->interrupt();

    	delete mResponseThread;
    	mResponseThread = NULL;
    }
        
	if (mNotifyThread != NULL) {
		mNotifyThread->interrupt();

		delete mNotifyThread;
		mNotifyThread = NULL;
	}
        
	if (mSSDPSocket != NULL) {
    	mSSDPSocket->close();

    	delete mSSDPSocket;
    	mSSDPSocket = NULL;
    }
}

void SsdpDeviceScanner::openSocket() {
	if (mSSDPSocket != NULL && mSSDPSocket->isConnected()) {
		return;
    }

    InetAddress *source = getIpAddress();
    if (source == NULL) {
        return;
    }
    LOGE("INIT SSDP SOCKET!");
    mSSDPSocket = new SSDPSocket(source);

    mSSDPSearchMsg = new SSDPSearchMsg();
}

InetAddress *SsdpDeviceScanner::getIpAddress() {
	InetAddress *address = new InetAddress();
	return address;
}

char *SsdpDeviceScanner::convertIpAddress(int ip) {
	return NULL;
}

void SsdpDeviceScanner::handleDatagramPacket(ParsedDatagram *pd) {

}

void SsdpDeviceScanner::getLocationData(string location, string uuid) {

}

void SsdpDeviceScanner::onResult(string uuid, LocationDevice *device) {

}

void *SsdpDeviceScanner::checkDeviceOffline(void *data) {
	LOGI("checkDeviceOffline!");
	return NULL;
}

void *SsdpDeviceScanner::sendSsdpSearchRequest(void *data) {
	SsdpDeviceScanner *scanner = (SsdpDeviceScanner*)data;

	printf("sendSsdpSearchRequest!data[%x][%s]\n",scanner->mSSDPSocket->isConnected(), scanner->mSSDPSearchMsg->toString().c_str());

	if (scanner->mSSDPSocket != NULL) {
		scanner->mSSDPSocket->send(scanner->mSSDPSearchMsg->toString());
	} else {
		LOGE("mSSDPSocket is NULL?!!");
	}

	return NULL;
}

void* SsdpDeviceScanner::handleResponse(void *data) {
	LOGE("SsdpDeviceScanner::handleResponse!!");

	return NULL;
}

void* SsdpDeviceScanner::handleResponseNotify(void *data) {
	LOGE("SsdpDeviceScanner::handleResponseNotify!!");

	return NULL;
}