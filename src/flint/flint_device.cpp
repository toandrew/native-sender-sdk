#include "flint_device.h"

FlintDevice::FlintDevice() {

}

FlintDevice::FlintDevice(const FlintDevice& device) {
	mHostAddress = device.mHostAddress;
	mVersionCode = device.mVersionCode;
	mDeviceId = device.mDeviceId;
	mFriendlyName = device.mFriendlyName;
	mModleName = device.mModleName;
	mDeviceVersion = device.mDeviceVersion;
	mServicePort = device.mServicePort;
	mFoundSource = device.mFoundSource;
}

FlintDevice::~FlintDevice() {

}

void FlintDevice::setDeviceId(string deviceId) {

}

void FlintDevice::setAddress(string address) {
	
}

void FlintDevice::setFriendlyName(string friendlyName) {

}

void FlintDevice::setModelName(string modelName) {

}

void FlintDevice::setDeviceVersion(string version) {

}

void FlintDevice::setServicePort(string port) {

}

void FlintDevice::setIconList() { // TODO
}

void FlintDevice::setFoundSource(string source) {

}

