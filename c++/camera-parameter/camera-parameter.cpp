#include "LIPSNICustomProperty.h"
#include <iostream>
#include <OpenNI.h>

using namespace std;
using namespace openni; 

void printStreamParameters(const string strSensorName, VideoStream &vs)
{
    int resX = 0, resY = 0;
    double fx = 0.0, fy = 0.0, cx = 0.0, cy = 0.0;
    float hfov = 0.0, vfov = 0.0;
    RadialDistortionCoeffs radialDistCoeffs;
    TangentialDistortionCoeffs tangentialDistCoeffs;
    int pos = 0;

    vs.getProperty(ONI_STREAM_PROPERTY_HORIZONTAL_FOV, &hfov);
    vs.getProperty(ONI_STREAM_PROPERTY_VERTICAL_FOV, &vfov);

    vs.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_X, &fx);
    vs.getProperty(LIPS_STREAM_PROPERTY_FOCAL_LENGTH_Y, &fy);

    vs.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_X, &cx);
    vs.getProperty(LIPS_STREAM_PROPERTY_PRINCIPAL_POINT_Y, &cy);

    vs.getProperty(LIPS_STREAM_PROPERTY_RADIAL_DISTORTION, &radialDistCoeffs);
    vs.getProperty(LIPS_STREAM_PROPERTY_TANGENTIAL_DISTORTION, &tangentialDistCoeffs);

    printf("\n Intrinsic of \"%s\"\n", strSensorName.c_str());
    printf("Fx:\t\t%f\n", fx);
    printf("Fy:\t\t%f\n", fy);
    printf("Cx:\t\t%f\n", cx);
    printf("Cy:\t\t%f\n", cy);
    printf("HFOV:\t\t%.2f\n", radian2deg(hfov));
    printf("VFOV:\t\t%.2f\n", radian2deg(vfov));
    printf("Distortion Coeffs:\n");
    printf("Radial:\tk1\tk2\tk3\tk4\tk5\n");
    printf("%f\t%f\t%f\t%f\t%f\n", radialDistCoeffs.k1, radialDistCoeffs.k2, radialDistCoeffs.k3, radialDistCoeffs.k4, radialDistCoeffs.k5);
    printf("Tangential:\tp1\tp2\n");
    printf("%f\t%f\n", tangentialDistCoeffs.p1, tangentialDistCoeffs.p2);
    printf("\n");
}

int main()
{
    if (OpenNI::initialize() != STATUS_OK)
    {
        std::cout << "Initialize Failed:" << OpenNI::getExtendedError() << std::endl;
        return -1;
    }

    Array<DeviceInfo> deviceList;
    OpenNI::enumerateDevices(&deviceList);

    if (deviceList.getSize() == 0)
    {
        std::cout << "Cannot find any camera\n";
        return -2;
    }

    Device device;
    if (device.open(deviceList[0].getUri()) != STATUS_OK)
    {
        std::cout << "Cannot open device:" << OpenNI::getExtendedError() << std::endl;
        return -3;
    }

    VideoStream color;
    color.create(device, SENSOR_COLOR);
    printStreamParameters("Color", color);

    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    printStreamParameters("Depth", depth);

    VideoStream ir;
    ir.create(device, SENSOR_IR);
    printStreamParameters("IR", ir);

    color.stop();
    device.close();
    OpenNI::shutdown();
}