#include <OpenNI.h>
#include <opencv2/core/core.hpp>

int main()
{
    auto version = new openni::Version();
    printf("OpenNI Version:%d.%d.%d.%d\n", version->build, version->major, version->minor, version->maintenance);

    printf("OpenCV Version:%s\n", CV_VERSION);
    return 0;
}