#ifndef FLINT_DEVICE_H_
#define FLINT_DEVICE_H_

#define FOUND_SOURCE_SSDP "ssdp"
#define FOUND_SOURCE_MDNS "mdns"

#include <string>
using namespace std;

class FlintDevice {
public:
	FlintDevice();
	FlintDevice(const FlintDevice& device);
	virtual ~FlintDevice();

	void setDeviceId(string deviceId);

	void setAddress(string address);

	void setFriendlyName(string friendlyName);

	void setModelName(string modelName);

	void setDeviceVersion(string version);

	void setServicePort(string port);

	void setIconList(); // TODO

	void setFoundSource(string source);

private:
    string mHostAddress;

    string mVersionCode;

    string mDeviceId;

    string mFriendlyName;

    string mModleName;

    string mDeviceVersion;

    string mServicePort;

    string mFoundSource;
};

#endif // FLINT_DEVICE_H_

