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

    cv::Mat overlappingMat;
    char input;
    bool enableRegistration = false;

    while (true)
    {
        color.readFrame(&colorFrame);
        depth.readFrame(&depthFrame);

        colorMat = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (void *)colorFrame.getData());
        cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());
        depthMat.convertTo(depthMat, CV_8U, 255.0 / 1024.0);
        cv::applyColorMap(depthMat, depthMat, cv::COLORMAP_JET);

        // In same camera model (e.g. L210/L215), color and depth resolution are different.
        // We need to crop one of them to make them the same.
        if (depthMat.rows != colorMat.rows)
        {
            colorMat = colorMat(cv::Rect(0, 0, depthMat.cols, depthMat.rows));
        }

        cv::addWeighted(colorMat, 0.8, depthMat, 0.2, 0.0, overlappingMat);

        cv::imshow("Align", overlappingMat);

        input = cv::waitKey(1);
        if (input == 'a')
        {
            enableRegistration = !enableRegistration;
            device.setImageRegistrationMode(enableRegistration ? openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR : openni::IMAGE_REGISTRATION_OFF);
            std::cout << "Image Registration: " << (enableRegistration ? "Enable" : "Disable") << std::endl;
        }
        else if (input == 'q')
        {
            break;
        }
    }

    color.stop();
    depth.stop();
    color.destroy();
    depth.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}