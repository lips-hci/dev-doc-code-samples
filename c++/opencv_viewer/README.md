# OpenCV Viewer

## Overview

This article shows how to convert OpenNI2 video frames to OpenCV Mat format. Once you have frame in Mat format, you can leverage all OpenCV functions.

## Expect Output

![](../../.asset/opencv_viewer.png)

## Tutorial

- After connect to camera and start each sensor (color, depth and IR). Read the frame into `VideoFrameRef`

```cpp
VideoFrameRef colorFrame;
VideoFrameRef depthFrame;
VideoFrameRef irFrame;

color.readFrame(&colorFrame);
depth.readFrame(&depthFrame);
ir.readFrame(&irFrame);
```

- Convert `VideoFrameRef` to `cv::Mat`

```cpp
// Color frame
cv::Mat colorMat = cv::Mat(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3, (void *)colorFrame.getData());

// Depth frame
cv::Mat depthMat = cv::Mat(depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1, (void *)depthFrame.getData());

// IR frame
cv::Mat irMat = cv::Mat(irFrame.getHeight(), irFrame.getWidth(), CV_16UC1, (void *)irFrame.getData());
```

- For better visualization, you can do the following conversion

```cpp
// For color frame
cv::cvtColor(colorMat, colorMat, cv::COLOR_BGR2RGB);

// For depth frame
depthMat.convertTo(depthMat, CV_8U, 255.0 / 1024.0);
cv::applyColorMap(depthMat, depthMat, cv::COLORMAP_JET);

// For IR frame
irMat.convertTo(irMat, CV_8U, 255.0 / 1024.0);
```

## Full code

[opencv_viewer.cpp](https://github.com/HedgeHao/LIPSedgeSDK_Tutorial/blob/master/c%2B%2B/opencv_viewer/opencv_viewer.cpp)
