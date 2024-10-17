#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include<C:\Users\admin\source\repos\plane_opengl\plane_opengl\model.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<C:\Users\admin\source\repos\plane_opengl\plane_opengl\shader.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"
using namespace std;



// 设置界面的高度和宽度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 照相机视野范围
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

//----------------------------函数定义

glm::mat4 flipModelVertically();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//窗口大小变化函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//鼠标滚轮缩放回调函数
void processInput(GLFWwindow* window);//输入函数

// 时间
float deltaTime = 0.0f; //当前帧与上一帧的时间差
float lastFrame = 0.0f;//上一帧时间


// 调用assimp实现模型加载
void loadModel(const std::string& path, std::vector<float>& vertices) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	aiMesh* mesh = scene->mMeshes[0];
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		if (mesh->mTextureCoords[0]) {
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}
}
//定义输入和加载纹理
void processInput(GLFWwindow* window);
GLuint loadTexture(const char* path);

// 长方体底座顶点数据
GLfloat vertices[] = {
	// Positions          // Texture Coords
		// Bottom face
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,  1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,  1.0f, 1.0f,
		 10.0f, 0.0f,  10.0f,  1.0f, 1.0f,
		-10.0f, 0.0f,  10.0f,  0.0f, 1.0f,
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,

		// Top face
		-10.0f, 0.5f, -10.0f,  0.0f, 0.0f,
		 10.0f, 0.5f, -10.0f,  1.0f, 0.0f,
		 10.0f, 0.5f,  10.0f,  1.0f, 1.0f,
		 10.0f, 0.5f,  10.0f,  1.0f, 1.0f,
		-10.0f, 0.5f,  10.0f,  0.0f, 1.0f,
		-10.0f, 0.5f, -10.0f,  0.0f, 0.0f,

		// Front face
		-10.0f, 0.0f, 10.0f,  0.0f, 0.0f,
		 10.0f, 0.0f, 10.0f,  1.0f, 0.0f,
		 10.0f, 0.5f, 10.0f,  1.0f, 1.0f,
		 10.0f, 0.5f, 10.0f,  1.0f, 1.0f,
		-10.0f, 0.5f, 10.0f,  0.0f, 1.0f,
		-10.0f, 0.0f, 10.0f,  0.0f, 0.0f,

		// Back face
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,  1.0f, 0.0f,
		 10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		 10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		-10.0f, 0.5f, -10.0f,  0.0f, 1.0f,
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,

		// Left face
		-10.0f, 0.0f,  10.0f,  0.0f, 0.0f,
		-10.0f, 0.0f, -10.0f,  1.0f, 0.0f,
		-10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		-10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		-10.0f, 0.5f,  10.0f,  0.0f, 1.0f,
		-10.0f, 0.0f,  10.0f,  0.0f, 0.0f,

		// Right face
		 10.0f, 0.0f,  10.0f,  0.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,  1.0f, 0.0f,
		 10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		 10.0f, 0.5f, -10.0f,  1.0f, 1.0f,
		 10.0f, 0.5f,  10.0f,  0.0f, 1.0f,
		 10.0f, 0.0f,  10.0f,  0.0f, 0.0f,

};




int main()
{
	// 初始化glfw
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw窗口创建
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);



	// glad: 加载所有OpenGL点
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// 在初始化之后，设置 ImGui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// 设置OpenGL视口,启用深度测试
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);


	// 加载飞机模型
	std::vector<float> planeVertices;
	loadModel("plane.stl", planeVertices);


	// 1. 设置飞机模型的VAO和VBO
	GLuint planeVBO, planeVAO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), &planeVertices[0], GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);




	// 2. 设置长方体底座顶点数据和缓冲
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// 加载和创建纹理
	GLuint texture = loadTexture("terrain.png");
	GLuint image_texture = loadTexture("wall.jpg"); // 使用stb_image或其他方法加载纹理

	// 加载着色器
	// -------------------------
	shader shader("shader_v.txt", "shader_f.txt");


	// 使模型以线条的形式呈现，可用可不用
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 每帧时间逻辑
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 处理输入
		processInput(window);

		// 渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 激活着色器
		shader.use();


		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



		// 绘制立方体底座
		//--------------------------------
		// 创建变换矩阵
		glBindTexture(GL_TEXTURE_2D, texture);//绑定纹理
		glm::mat4 model = glm::mat4(1.0f);
		float heightOffset = sin(glfwGetTime()) * 0.2f; // 动态高度变化
		model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));//缩放模型
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//绑定
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		//同时创建五个飞机模型
		for (int i = 0; i < 5; i++) {
			model = glm::mat4(1.0f);
			float angle = glm::radians(72.0f * i + (float)glfwGetTime() * -50.0f); //逆时针方向旋转
			float radius = 8.0f; // 确保飞机模型在长方体底座内
			float x = radius * cos(angle);
			float z = radius * sin(angle);
			model = glm::translate(model, glm::vec3(x, 1.5f, z)); // 调整飞机飞行高度
			model = glm::scale(model, glm::vec3(0.015f)); // 调整飞机大小
			glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, planeVertices.size() / 5);
			glBindVertexArray(0);

		}

		//glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH - 100, SCR_HEIGHT - 100), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Always);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));  // 设置窗口背景为透明
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);  // 去掉窗口边框
		ImGui::Begin("image_texture", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		
		ImGui::Image((void*)(intptr_t)image_texture, ImVec2(100, 100));



		ImGui::End();
		ImGui::PopStyleVar();  // 恢复默认样式
		ImGui::PopStyleColor();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// 交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// glfw: 清除glfw分配的所有资源
	glfwTerminate();
	return 0;
}
//输入函数
void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
//加载纹理函数
GLuint loadTexture(const char* path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
// 窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//鼠标回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
//鼠标滚动回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

glm::mat4 flipModelVertically() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	return model;
}