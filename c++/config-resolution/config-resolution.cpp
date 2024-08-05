#include <iostream>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace openni;

std::string pixelFormatToString(PixelFormat format)
{
    switch (format)
    {
    // Depth
    case PIXEL_FORMAT_DEPTH_100_UM:
        return "Depth-16bit,0.1mm";
        break;
    case PIXEL_FORMAT_DEPTH_1_MM:
        return "Depth-16bit,1mm";
        break;

    // Color
    case PIXEL_FORMAT_RGB888:
        return "RGB888";
        break;
    case PIXEL_FORMAT_YUV422:
        return "YUV422";
        break;
    case PIXEL_FORMAT_JPEG:
        return "JPEG";
        break;
    case PIXEL_FORMAT_YUYV:
        return "YUYV";
        break;

    // Infrared
    case PIXEL_FORMAT_GRAY8:
        return "Grayscale-8bit";
        break;
    case PIXEL_FORMAT_GRAY16:
        return "Grayscale-16bit";
        break;
    default:
        return "Unknown";
        break;
    }
}

int main()
{
    OpenNI::initialize();

    Device device;
    if (device.open(ANY_DEVICE) != STATUS_OK)
    {
        std::cout << "No device connect\n";
        return -1;
    }

    const openni::SensorInfo *colorInfo = device.getSensorInfo(SENSOR_COLOR);
    const openni::SensorInfo *depthInfo = device.getSensorInfo(SENSOR_DEPTH);
    const openni::SensorInfo *irInfo = device.getSensorInfo(SENSOR_IR);

    auto &colorVideoModes = colorInfo->getSupportedVideoModes();
    auto &depthVideoModes = depthInfo->getSupportedVideoModes();
    auto &irVideoModes = irInfo->getSupportedVideoModes();

    std::cout << "\n====== Available Video Mode] (width, height, FPS, Format) ======\n";
    std::cout << "[Color]\n";
    for (int i = 0; i < colorVideoModes.getSize(); i++)
    {
        std::cout << i << ":" << colorVideoModes[i].getResolutionX() << "," << colorVideoModes[i].getResolutionY() << "," << colorVideoModes[i].getFps() << "," << pixelFormatToString(colorVideoModes[i].getPixelFormat()) << std::endl;
    }

    std::cout << "\n[Depth]\n";
    for (int i = 0; i < depthVideoModes.getSize(); i++)
    {
        std::cout << i << ":" << depthVideoModes[i].getResolutionX() << "," << depthVideoModes[i].getResolutionY() << "," << depthVideoModes[i].getFps() << "," << pixelFormatToString(depthVideoModes[i].getPixelFormat()) << std::endl;
    }

    std::cout << "\n[IR]\n";
    for (int i = 0; i < irVideoModes.getSize(); i++)
    {
        std::cout << i << ":" << irVideoModes[i].getResolutionX() << "," << irVideoModes[i].getResolutionY() << "," << irVideoModes[i].getFps() << "," << pixelFormatToString(irVideoModes[i].getPixelFormat()) << std::endl;
    }
    std::cout << "\n";

    std::string input;
    int colorSelection = 0, depthSelection = 0, irSelection = 0;
    
    std::cout << "Select Color video mode:";
    std::cin >> input;
    colorSelection = atoi(input.c_str());

    std::cout << "Select Depth video mode:";
    std::cin >> input;
    depthSelection = atoi(input.c_str());

    std::cout << "Select Ir video mode:";
    std::cin >> input;
    irSelection = atoi(input.c_str());

    // Color
    VideoStream color;
    color.create(device, SENSOR_COLOR);
    color.setVideoMode(colorVideoModes[colorSelection]);
    color.start();
    VideoFrameRef colorFrame;
    cv::Mat colorMat;

    // Depth
    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    depth.setVideoMode(depthVideoModes[depthSelection]);
    depth.start();
    VideoFrameRef depthFrame;
    cv::Mat depthMat;

    // IR
    VideoStream ir;
    ir.create(device, SENSOR_IR);
    ir.setVideoMode(irVideoModes[irSelection]);
    ir.start();
    VideoFrameRef irFrame;
    cv::Mat irMat;

    VideoMode colorMode = color.getVideoMode();
    VideoMode depthMode = depth.getVideoMode();
    VideoMode irMode = ir.getVideoMode();

    std::cout << "\nCurrent Video Mode\n";
    std::cout << "[Color]:" << colorMode.getResolutionX() << "," << colorMode.getResolutionY() << "," << colorMode.getFps() << "," << pixelFormatToString(colorMode.getPixelFormat()) << std::endl;
    std::cout << "[Depth]:" << depthMode.getResolutionX() << "," << depthMode.getResolutionY() << "," << depthMode.getFps() << "," << pixelFormatToString(depthMode.getPixelFormat()) << std::endl;
    std::cout << "[IR]:" << irMode.getResolutionX() << "," << irMode.getResolutionY() << "," << irMode.getFps() << "," << pixelFormatToString(irMode.getPixelFormat()) << std::endl;

    while (cv::waitKey(1) != 'q')
    {
        color.readFrame(&colorFrame);
        depth.readFrame(&depthFrame);
        ir.readFrame(&irFrame);

        colorMat = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (void *)colorFrame.getData());
        cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());
        depthMat.convertTo(depthMat, CV_8U, 255.0 / 1024.0);
        cv::applyColorMap(depthMat, depthMat, cv::COLORMAP_JET);

        irMat = cv::Mat(irFrame.getHeight(), irFrame.getWidth(), CV_16UC1, (void *)irFrame.getData());
        irMat.convertTo(irMat, CV_8U, 255.0 / 1024.0);

        cv::imshow("Color", colorMat);
        cv::imshow("Depth", depthMat);
        cv::imshow("IR", irMat);
    }

    color.stop();
    depth.stop();
    ir.stop();
    color.destroy();
    depth.destroy();
    ir.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}