#include "mouseOperation.h"
#include<camera.h>

bool mouseOperation::firstMouse = true;
float mouseOperation::lastX = 400.0f;
float mouseOperation::lastY = 300.0f;
float mouseOperation::yaw = 90.0f;
float mouseOperation::pitch = 0.0f;
float mouseOperation::sensitivity = 0.1f;
float mouseOperation::fov = 45.0f;
// camera





void mouseOperation::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
}
//鼠标滚动回调函数
void mouseOperation::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
