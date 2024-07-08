# Environment Setup

## Overview
This example help you make sure all the development environment is set up correctly

## Expect Output
* It's okay if the OpenNI Version is 0.0.0.0
```
OpenNI Version:0.0.0.0
OpenCV Version:4.2.0.4.2.0
```

## Tutorial
* This program simply include OpenCV and OpenNI headers then print the library version
```cpp
    auto version = new openni::Version();
    printf("OpenNI Version:%d.%d.%d.%d\n", version->build, version->major, version->minor, version->maintenance);
    printf("OpenCV Version:%s\n", CV_VERSION);
```

## Full code

[environment_setup.cpp](./environment_setup.cpp)