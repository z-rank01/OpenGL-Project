#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm/glm.hpp>

#include <glad/glad.h>


class Shader
{
public:
	unsigned int ID;
	
	Shader(const char* vPath, const char* fPath)
	{
		std::string vCode;
		std::string fCode;
		std::ifstream vShaderfile;
		std::ifstream fShaderfile;

		vShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderfile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		//Read Shader file and load it in buffer.
		try
		{
			vShaderfile.open(vPath);
			fShaderfile.open(fPath);
			std::stringstream vShaderStream, fShaderStream;
			
			vShaderStream << vShaderfile.rdbuf();
			fShaderStream << fShaderfile.rdbuf();

			vShaderfile.close();
			fShaderfile.close();
			
			vCode = vShaderStream.str();
			fCode = fShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "Error shader file not successfully read" << std::endl;
		}
		const char* vShaderCode = vCode.c_str();
		const char* fShaderCode = fCode.c_str();

		//Create Shader.
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//Create Program.
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		//Release Buffer.
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	
	void use()
	{
		glUseProgram(ID);
	}
	
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

};

#endif