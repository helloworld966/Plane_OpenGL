#ifndef MYSHADER_H
#define MYSHADER_H

#include <glad/glad.h> 
#include<string>

class shader {
public:
	GLuint shaderProgram;
	shader(const char* vertexPath, const char* fragmentPath);
	void use();
private:
	std::string readFile(const char* filePath);
	void compileShader(const GLchar vertex_shader_source[], const GLchar fragment_shader_source[]);
};
#endif // !SHADER_H
