#include <iostream>
#include <OpenNI.h>

int main()
{
	if (openni::OpenNI::initialize() != openni::STATUS_OK)
	{
		std::cout << "Initialize Failed:" << openni::OpenNI::getExtendedError() << std::endl;
		return -1;
	}

	openni::Array<openni::DeviceInfo> deviceList;
	openni::OpenNI::enumerateDevices(&deviceList);

	if (deviceList.getSize() == 0)
	{
		std::cout << "Cannot find any camera\n";
		return -2;
	}

	for (int i = 0; i < deviceList.getSize(); i++)
	{
		openni::DeviceInfo info = deviceList[i];
		std::cout << "Name:" << info.getName() << std::endl;
		std::cout << "Uri:" << info.getUri() << std::endl;
		std::cout << "USB Product ID:" << info.getUsbProductId() << std::endl;
		std::cout << "USB Vendor ID:" << info.getUsbVendorId() << std::endl;
		std::cout << "Vendor:" << info.getVendor() << std::endl;
	}

	openni::Device device;
	if (device.open(deviceList[0].getUri()) != openni::STATUS_OK)
	{
		std::cout << "Cannot open device:" << openni::OpenNI::getExtendedError() << std::endl;
		return -3;
	}

	device.close();
	openni::OpenNI::shutdown();
}