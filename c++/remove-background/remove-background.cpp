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

    device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

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
    double thresholdValueUp = 530;
    double thresholdValueDown = 385;
    char input;

    while (true)
    {
        color.readFrame(&colorFrame);
        depth.readFrame(&depthFrame);

        colorMat = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (void *)colorFrame.getData());
        cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());

        if (depthMat.rows != colorMat.rows)
        {
            colorMat = colorMat(cv::Rect(0, 0, depthMat.cols, depthMat.rows));
        }

        depthMat.copyTo(thres);
        cv::threshold(thres, thres, thresholdValueDown, 1024, cv::THRESH_TOZERO);
        cv::threshold(thres, thres, thresholdValueUp, 1024, cv::THRESH_TOZERO_INV);
        cv::threshold(thres, thres, 1, 1024, cv::THRESH_BINARY);
        thres.convertTo(thres, CV_8UC1, 255.0 / 1024.0);

        cv::Mat res;
        cv::bitwise_and(colorMat, colorMat, res, thres);

        cv::imshow("Origin", colorMat);
        cv::imshow("Thres", thres);
        cv::imshow("Color", res);

        input = cv::waitKey(1);
        if (input == 'p')
        {
            if (thresholdValueUp + 5 <= 2000)
                thresholdValueUp += 5;
            else
                thresholdValueUp = 2000;

            std::cout << "Threshold Value (Up):" << thresholdValueUp << " (Down):" << thresholdValueDown << std::endl;
        }
        else if (input == 'o')
        {
            if (thresholdValueUp - 5 >= 0 && thresholdValueUp - 5 >= thresholdValueDown)
                thresholdValueUp -= 5;

            std::cout << "Threshold Value (Up):" << thresholdValueUp << " (Down):" << thresholdValueDown << std::endl;
        }
        else if (input == 'x')
        {
            if (thresholdValueDown + 5 <= 2000 && thresholdValueDown + 5 <= thresholdValueUp)
                thresholdValueDown += 5;

            std::cout << "Threshold Value (Up):" << thresholdValueUp << " (Down):" << thresholdValueDown << std::endl;
        }
        else if (input == 'z')
        {
            if (thresholdValueDown - 5 >= 0)
                thresholdValueDown -= 5;
            else
                thresholdValueDown = 0;

            std::cout << "Threshold Value (Up):" << thresholdValueUp << " (Down):" << thresholdValueDown << std::endl;
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