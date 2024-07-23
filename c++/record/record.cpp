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

    // IR
    VideoStream ir;
    ir.create(device, SENSOR_IR);
    ir.start();
    VideoFrameRef irFrame;
    cv::Mat irMat;

    Recorder recorder;
    if (recorder.create("record.oni") != STATUS_OK)
    {
        std::cout << "Cannot create recorder" << std::endl;
        return -2;
    }
    recorder.attach(color);
    recorder.attach(depth);
    recorder.attach(ir);

    char input;
    bool isRecording = false;
    while (true)
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

        input = cv::waitKey(1);
        if (input == 'q')
        {
            break;
        }
        else if (input == 'r')
        {
            if (!isRecording)
            {
                recorder.start();
                std::cout << "Start recording" << std::endl;
            }
        }
        else if (input == 's')
        {
            if (isRecording)
            {
                recorder.stop();
                std::cout << "Stop record" << std::endl;
            }
        }
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