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

    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    depth.start();
    
    VideoFrameRef depthFrame;
    cv::Mat depthMat;
    cv::Mat depthMatRoi;
    cv::Rect roi(160, 80, 320, 240);

    while (cv::waitKey(1) != 'q')
    {
        depth.readFrame(&depthFrame);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());
        depthMat.convertTo(depthMat, CV_8U, 255.0 / 1024.0);
        cv::applyColorMap(depthMat, depthMat, cv::COLORMAP_JET);

        depthMatRoi = depthMat(roi);

        cv::imshow("Depth", depthMat);
        cv::imshow("ROI", depthMatRoi);
    }

    depth.stop();
    depth.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}