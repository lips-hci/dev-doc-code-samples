#include <iostream>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace openni;

int main()
{
    OpenNI::initialize();

    Device device;
    if (device.open(ANY_DEVICE) != STATUS_OK)
    {
        std::cout << "No device connect\n";
        return -1;
    }

    // Color
    VideoStream color;
    color.create(device, SENSOR_COLOR);
    color.start();
    VideoFrameRef colorFrame;
    cv::Mat colorMat;

    // Depth
    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    depth.start();
    VideoFrameRef depthFrame;
    cv::Mat depthMat;

    cv::Mat thres;
    double thresholdValueUp = 1500;
    double thresholdValueDown = 500;

    while (cv::waitKey(1) != 'q')
    {
        color.readFrame(&colorFrame);
        depth.readFrame(&depthFrame);

        colorMat = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (void *)colorFrame.getData());
        cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());

        depthMat.copyTo(thres);
        cv::threshold(thres, thres, thresholdValueDown, 1024, cv::THRESH_TOZERO);
        cv::threshold(thres, thres, thresholdValueUp, 1024, cv::THRESH_TOZERO_INV);
        cv::threshold(thres, thres, 1, 1024, cv::THRESH_BINARY_INV);

        depthMat.convertTo(depthMat, CV_8UC1, 255.0 / 1024.0);
        thres.convertTo(thres, CV_8UC1, 255.0 / 1024.0);

        cv::imshow("Color", colorMat);
        cv::imshow("Depth", depthMat);
        cv::imshow("Threshold", thres);
    }

    color.stop();
    depth.stop();
    color.destroy();
    depth.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}