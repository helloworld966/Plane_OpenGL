#include "shader.h"
#include<iostream>
#include<fstream>
#include<sstream>

shader::shader(const char*vertexPath, const char* fragmentPath) {
	std::string vertexCode = readFile(vertexPath);
	std::string fragmentCode = readFile(fragmentPath);
	compileShader(vertexCode.c_str(), fragmentCode.c_str());
}

std::string shader::readFile(const char* filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "fail to open file: " << filePath << std::endl;
		return "";
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

void shader::compileShader(const GLchar vertex_shader_source[], const GLchar fragment_shader_source[])
{
	GLuint vertexShader, fragmentShader;
	GLchar infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
}

void shader::use()
{
	glUseProgram(shaderProgram);
}