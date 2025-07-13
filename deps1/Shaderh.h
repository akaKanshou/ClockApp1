#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int program;
	Shader() : program(0) {};
	Shader(const char* vertexsource, const char* fragmentsource) : program(0) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vsFile;
		std::ifstream fsFile;
		vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vsFile.open(vertexsource);
			fsFile.open(fragmentsource);

			std::stringstream vsStream, fsStream;
			vsStream << vsFile.rdbuf();
			fsStream << fsFile.rdbuf();

			vsFile.close();
			fsFile.close();

			vertexCode = vsStream.str();
			fragmentCode = fsStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vsShaderSource = vertexCode.c_str();
		const char* fsShaderSource = fragmentCode.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexShader, 1, &vsShaderSource, NULL);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, &fsShaderSource, NULL);
		glCompileShader(fragmentShader);

		int success; char infoLogV[512], infoLogF[512], infoLogP[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLogV);
			std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILURE\n" << infoLogV << std::endl;
		}

		success = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogF);
			std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_ERROR\n" << infoLogF << std::endl;
		}

		success = 0;


		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLogP);
			std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILURE\n" << infoLogP << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use() const {
		if (!program) {
			std::cout << "Invalid_Shader_Used\n";
			throw;
		}
		glUseProgram(program);
	}

	void setInt(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
	}

	void setFloat(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);
	}

	void setVec3(const std::string& name, float a, float b, float c) {
		glUniform3f(glGetUniformLocation(program, name.c_str()), a,b,c);
	}

	void setVec4(const std::string& name, float a, float b, float c, float d) {
		glUniform4f(glGetUniformLocation(program, name.c_str()), a,b,c,d);	
	}

	void setVec2(const std::string& name, float a, float b) {
		glUniform2f(glGetUniformLocation(program, name.c_str()), a, b);
	}

	void setMat4(const std::string& name, glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

};
#endif // !SHADER_H
