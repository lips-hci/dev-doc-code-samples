#include <iostream>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define WINDOW_NAME "Depth_data"

using namespace openni;

cv::Mat depthMat;

void onMouseMove(int event, int x, int y, int flags, void *userdata)
{
    if (event == cv::EVENT_MOUSEMOVE)
    {
        uint16_t *depthData = (uint16_t *)depthMat.data;
        uint16_t depthValue = depthData[y * depthMat.cols + x];
        std::cout << "Depth value at (" << x << "," << y << ") = " << depthValue << std::endl;
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

    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    depth.start();

    VideoFrameRef depthFrame;
    cv::Mat depthMatDisplay;
    cv::namedWindow(WINDOW_NAME);
    cv::setMouseCallback(WINDOW_NAME, onMouseMove, NULL);

    while (cv::waitKey(1) != 'q')
    {
        depth.readFrame(&depthFrame);

        depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());
        depthMat.convertTo(depthMatDisplay, CV_8U, 255.0 / 1024.0);
        cv::applyColorMap(depthMatDisplay, depthMatDisplay, cv::COLORMAP_JET);

        cv::imshow(WINDOW_NAME, depthMatDisplay);
    }

    depth.stop();
    depth.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}