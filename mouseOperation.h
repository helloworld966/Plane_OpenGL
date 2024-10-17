#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>
#include<camera.h>
class mouseOperation {
public:
	static bool firstMouse;
	static float lastX, lastY;
	static float yaw, pitch;
	static float sensitivity;
	static float fov;
	static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


};

#endif