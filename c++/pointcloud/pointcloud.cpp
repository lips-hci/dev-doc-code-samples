#include <iostream>
#include <OpenNI.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

using namespace openni;

struct pointData
{
    unsigned short depth;
    float worldX, worldY, worldZ;
    float r, g, b;
};

struct viewerState
{
    double yaw;
    double pitch;
    double lastX;
    double lastY;
    float offset;
    float lookatX;
    float lookatY;
    bool mouseLeft;
    bool mouseRight;
};

struct pointData *pointsData;
struct viewerState viewerStat;

void computeCloud(int width, int height, VideoStream &depth, VideoStream &color, VideoFrameRef &colorFrame, VideoFrameRef &depthFrame)
{
    color.readFrame(&colorFrame);
    depth.readFrame(&depthFrame);

    const openni::DepthPixel *pDepth = (const openni::DepthPixel *)depthFrame.getData();
    const openni::RGB888Pixel *pColor = (const openni::RGB888Pixel *)colorFrame.getData();

    float fX, fY, fZ;

    for (int y = 0, i = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            fX = 0.0;
            fY = 0.0;
            fZ = 0.0;
            pointsData[i].depth = pDepth[i];

            if (pDepth[i] != 0)
            {

                openni::CoordinateConverter::convertDepthToWorld(depth, x, y, pDepth[i], &fX, &fY, &fZ);

                pointsData[i].worldX = fX;
                pointsData[i].worldY = fY;
                pointsData[i].worldZ = fZ;

                pointsData[i].r = pColor[i].r / 255.0;
                pointsData[i].g = pColor[i].g / 255.0;
                pointsData[i].b = pColor[i].b / 255.0;
            }
            i++;
        }
    }

    return;
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (viewerStat.mouseLeft)
    {
        viewerStat.yaw -= (xpos - viewerStat.lastX);
        viewerStat.pitch += (ypos - viewerStat.lastY);
    }

    if (viewerStat.mouseRight)
    {
        viewerStat.lookatX += (xpos - viewerStat.lastX);
        viewerStat.lookatY += (ypos - viewerStat.lastY);
    }

    viewerStat.lastX = xpos;
    viewerStat.lastY = ypos;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_SPACE)
        {
            viewerStat.yaw = 0.0;
            viewerStat.pitch = 0.0;
            viewerStat.offset = 0.0;
            viewerStat.lookatX = 0.0;
            viewerStat.lookatY = 0.0;
        }
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_RIGHT)
        {
            viewerStat.yaw -= 1.0f;
        }
        else if (key == GLFW_KEY_LEFT)
        {
            viewerStat.yaw += 1.0f;
        }
        else if (key == GLFW_KEY_DOWN)
        {
            viewerStat.pitch += 1.0f;
        }
        else if (key == GLFW_KEY_UP)
        {
            viewerStat.pitch -= 1.0f;
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        viewerStat.mouseLeft = true;
    }
    else
    {
        viewerStat.mouseLeft = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        viewerStat.mouseRight = true;
    }
    else
    {
        viewerStat.mouseRight = false;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    viewerStat.offset -= static_cast<float>(yoffset * 25);
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

    device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

    // Color
    VideoStream color;
    color.create(device, SENSOR_COLOR);
    color.start();
    VideoFrameRef colorFrame;

    // Depth
    VideoStream depth;
    depth.create(device, SENSOR_DEPTH);
    depth.start();
    VideoFrameRef depthFrame;

    const int depthWidth = depth.getVideoMode().getResolutionX();
    const int depthHeight = depth.getVideoMode().getResolutionY();

    pointsData = new struct pointData[depthWidth * depthHeight];

    glfwInit();
    GLFWwindow *window = glfwCreateWindow(1280, 720, "PointCloud", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    viewerStat.yaw = 0.0;
    viewerStat.pitch = 0.0;
    viewerStat.lastX = 0.0;
    viewerStat.lastY = 0.0;
    viewerStat.offset = 0.0;
    viewerStat.lookatX = 0.0;
    viewerStat.lookatY = 0.0;
    viewerStat.mouseLeft = false;
    viewerStat.mouseRight = false;

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        computeCloud(depthWidth, depthHeight, depth, color, colorFrame, depthFrame);

        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, (float)windowWidth / windowHeight, 0.01f, 100000.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(viewerStat.lookatX, viewerStat.lookatY, 0, viewerStat.lookatX, viewerStat.lookatY, 1, 0, 1, 0);

        glPointSize(windowWidth / 640.0);
        glEnable(GL_DEPTH_TEST);

        glTranslatef(0, 0, viewerStat.offset);
        glRotated(viewerStat.pitch, 1, 0, 0);
        glRotated(viewerStat.yaw, 0, 1, 0);
        glTranslatef(0, 0, -0.5f);

        glBegin(GL_POINTS);
        for (int i = 0; i < (depthWidth * depthHeight); i++)
        {
            if (pointsData[i].depth != 0)
            {
                glColor3f(pointsData[i].r, pointsData[i].g, pointsData[i].b);
                glVertex3f(pointsData[i].worldX, pointsData[i].worldY, pointsData[i].worldZ);
            }
        }
        glEnd();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    color.stop();
    depth.stop();
    color.destroy();
    depth.destroy();
    device.close();
    OpenNI::shutdown();

    return 0;
}