# Hello LIPSEdge SDK

## Overview

This example shows how to connect to camera and get device info.

## Expect Output

```
Name:TS800
Uri:USB#VID_2DF2&PID_0213&REV_0007&BUS_02&ADR_10
USB Product ID:531
USB Vendor ID:11762
Vendor:LIPS
```

## Tutorial

- Import openni header

```cpp
#include <openni2/OpenNI.h>
```

- Initialize SDK

```cpp
if (openni::OpenNI::initialize() != openni::STATUS_OK)
	{
		std::cout <<  "Initialize Failed:" << openni::OpenNI::getExtendedError() << std::endl;
		return -1;
	}
```

- Search for connected devices

```cpp
openni::Array<openni::DeviceInfo> deviceList;
openni::OpenNI::enumerateDevices(&deviceList);
```

- Connect to camera and get camera device info

> Or you can use `open(openni::ANY_DEVICE)` to connect to the first found device.

```cpp
for (int i = 0; i < deviceList.getSize(); i++)
{
	openni::DeviceInfo info = deviceList[i];
	std::cout << "Name:" << info.getName() << std::endl;
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
```

- Close camera and deconstruct OpenNI SDK

```cpp
device.close();
openni::OpenNI::shutdown();
```

## Full code

[hello-lipsedge-sdk.cpp]()