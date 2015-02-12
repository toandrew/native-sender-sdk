#include "ssdp_device_scanner.h"
#include <regex.h>

#include "tinyxml2.h"

using namespace tinyxml2; 

#define MAX_NUM_OF_THREADS 20

#define SSDP_RESPONSE_USNKEY_UUID_PATTERN "uuid:(.+)(::.+)"

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

	stop();

	mStarting = true;

	openSocket();

	mSSDPSearchMsg = new SSDPSearchMsg();

	mCheckOfflineTimer = new Timer();
	mCheckOfflineTimer->schedule(checkDeviceOffline, 1, SsdpDeviceScanner::RESCAN_INTERVAL, (void *)this);

	mSendSearchTimer = new Timer();
	mSendSearchTimer->schedule(sendSsdpSearchRequest, 1, SsdpDeviceScanner::RESCAN_INTERVAL, (void *)this);

	mResponseHandler = new Runnable(handleResponse);
	mResponseThread = new Thread(mResponseHandler, (void*)this);

    mRespNotifyHandler = new Runnable(handleResponseNotify);
	mNotifyThread = new Thread(mRespNotifyHandler, (void *)this);

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
	InetAddress *address = new InetAddress("0.0.0.0");
	return address;
}

char *SsdpDeviceScanner::convertIpAddress(int ip) {
	return NULL;
}

void SsdpDeviceScanner::handleDatagramPacket(ParsedDatagram *pd) {
	map<string, string> data = pd->getData();

	string serviceFilter;
	string type = pd->getType();
	if (type == SL_NOTIFY) {
		serviceFilter = data[NT];
	} else {
		serviceFilter = data[ST];
	}

	if ("urn:dial-multiscreen-org:service:dial:1" != serviceFilter) {
		LOGE("handleDatagramPacket: not valid!");
		return;
	}

	string usnKey = data[USN];
	printf("USNKEY:%s\n",usnKey.c_str());
	if (usnKey.length() == 0) {
		LOGE("handleDatagramPacket: usnKey is NULL!");
		return;
	}

    int nErrCode = 0;
    regmatch_t pmatch[2];
    regex_t oRegex;
    if ((nErrCode = regcomp(&oRegex, SSDP_RESPONSE_USNKEY_UUID_PATTERN, REG_EXTENDED)) != 0
     	|| ((nErrCode = regexec(&oRegex, usnKey.c_str(), 2, pmatch, REG_NOTBOL)) != 0)) {
		LOGE("FAILED TO EXTRACT UUID!");
	    char szErrMsg[1024] = {0};
	    size_t unErrMsgLen = 0;
	    unErrMsgLen = regerror(nErrCode, &oRegex, szErrMsg, sizeof(szErrMsg));
	    unErrMsgLen = unErrMsgLen < sizeof(szErrMsg) ? unErrMsgLen : sizeof(szErrMsg) - 1;
	    szErrMsg[unErrMsgLen] = '\0';
	    printf("ErrMsg: %s\n", szErrMsg);
	    regfree(&oRegex);

 	}

    regfree(&oRegex);

    printf("start[%d]end[%d]\n",pmatch[1].rm_so, pmatch[1].rm_eo);

    string uuid = usnKey.substr(pmatch[1].rm_so, pmatch[1].rm_eo - pmatch[1].rm_so);

    printf("%s matches %s [%s]\n", SSDP_RESPONSE_USNKEY_UUID_PATTERN, usnKey.c_str(), uuid.c_str());

    if (NTS_BYEBYE == data[NTS]) {
    	printf("byebye uuid = %s\n", uuid.c_str());
    	string deviceId = mFoundDeviceMap[uuid];
    	setDeviceOffline(deviceId);
    } else {
    	string location = data[LOCATION];
    	if (location.length() == 0) {
    		LOGE("LOCATION IS NULL!IGNORE!");
    		return;
    	}

    	bool found = false;
    	list<string>::iterator it = find(mDiscoveredDeviceList.begin(), mDiscoveredDeviceList.end(), uuid);
    	if (it != mDiscoveredDeviceList.end()) {
    		found = true;
    	}

    	if (!found && mFoundDeviceMap[uuid].empty()) {
    		printf("insert into device list[%s]\n", uuid.c_str());
    		mDiscoveredDeviceList.push_back(uuid);
    		getLocationData(location, uuid);
    	} else {
    		printf("update device list[%s]\n", uuid.c_str());
    		string deviceId = mFoundDeviceMap[uuid];
    		if (!deviceId.empty()) {
    			//synchronized (mScannerData) {
    				//if (mScannerData[deviceId].length() > 0) {
    					//((SsdpScannerData) mScannerData[deviceId]).mScannedTime = SystemClock.elapsedRealtime();
    				//}
    			//}
    		}
    	}

    }

}

void SsdpDeviceScanner::getLocationData(string location, string uuid) {
	printf("getLocationData![%s][%s]\n", location.c_str(), uuid.c_str());

	if (mLocationDevice == NULL) {
		mLocationDevice = new LocationDevice();
	}

	if (mHttpClient == NULL) {
		mHttpClient = new CHttpClient();
	}

	string response;

	mHttpClient->Get(location, response);
	printf("getLocationData and response:[%s]\n", response.c_str());

	XMLDocument doc;
	doc.Parse(response.c_str());

	XMLElement *root = doc.RootElement();
	XMLElement *URLBase = root->FirstChildElement("URLBase");
	if (URLBase != NULL) {
		mLocationDevice->url = URLBase->GetText();
		printf( "URLBase[%s]\n", URLBase->GetText());
	}

	XMLElement *device = root->FirstChildElement("device");
	if (device != NULL) {
		XMLElement *friendlyName = device->FirstChildElement("friendlyName");
		if (friendlyName) {
			mLocationDevice->friendlyName = friendlyName->GetText();
			printf("friendlyName![%s]\n", mLocationDevice->friendlyName.c_str());
		}
		
		XMLElement *manufacturer = device->FirstChildElement("manufacturer");
		if (manufacturer) {
			mLocationDevice->manufacturer = manufacturer->GetText();
			printf("manufacturer![%s]\n", mLocationDevice->manufacturer.c_str());
		}
		
		XMLElement *modelName = device->FirstChildElement("modelName");
		if (modelName) {
			mLocationDevice->modelName = modelName->GetText();
			printf("modelName![%s]\n", mLocationDevice->modelName.c_str());
		}

		onResult(uuid, mLocationDevice);
	}

	// ignore icons


}

void SsdpDeviceScanner::onResult(string uuid, LocationDevice *device) {
	printf("onResult![%s]\n", uuid.c_str());
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
	LOGE("ENTER SsdpDeviceScanner::handleResponse!!");
	SsdpDeviceScanner *scanner = (SsdpDeviceScanner*)data;

	if (scanner == NULL) {
		LOGE("handleResponse: scanner IS NULL!");
	} else {
		printf("handleResponse: scanner IS NOT NULL![%d]\n", scanner->mStarting);
	}
	while(scanner != NULL && scanner->mStarting) {
		DatagramPacket *dp = NULL;
		if (scanner->mSSDPSocket != NULL) {
			dp = scanner->mSSDPSocket->responseReceive();
			if (dp != NULL) {
				LOGE("RECEIVED RESPONSE!");

				scanner->handleDatagramPacket(new ParsedDatagram(dp));
			}
		}
	}

	LOGE("QUIT SsdpDeviceScanner::handleResponse!");

	return NULL;
}

void* SsdpDeviceScanner::handleResponseNotify(void *data) {
	LOGE("ENTER SsdpDeviceScanner::handleResponseNotify!!");
	SsdpDeviceScanner *scanner = (SsdpDeviceScanner*)data;

	if (scanner == NULL) {
		LOGE("handleResponseNotify: scanner IS NULL!");
	} else {
		printf("handleResponseNotify: scanner IS NOT NULL![%d]\n",scanner->mStarting);
	}

	while(scanner != NULL && scanner->mStarting) {
		DatagramPacket *dp = NULL;
		if (scanner->mSSDPSocket != NULL) {
			dp = scanner->mSSDPSocket->notifyReceive();
			LOGE("RECEIVED NOTIFY!");
			if (dp != NULL) {
				LOGE("RECEIVED RESPONSE!");

				scanner->handleDatagramPacket(new ParsedDatagram(dp));
			}
		}
	}
	
	LOGE("QUIT SsdpDeviceScanner::handleResponseNotify!!");

	return NULL;
}
