#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
public:
	//ID do programa
	unsigned int ID;

	//Construtor que lê e da build no shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
		//Pega o codigo fonte do filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		try{//tenta abrir os arquivos
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//fecha os file handlers
			vShaderFile.close();
			fShaderFile.close();
			//Converte stream em string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e){
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char *vShaderCode = vertexCode.c_str();
		const char *fShaderCode = fragmentCode.c_str();
		//compila os shaders
		unsigned int vertex, fragment;
		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		//deleta os shaders que n são mais necessarios
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	//Ativa o shader
	void use(){
		glUseProgram(ID);
	}
	//Funções utilitarias
	void setBool(const std::string &name, bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
private:
	//Checa erros de compilaão ou linkagem	
	void checkCompileErrors(unsigned int shader, std::string type){
		int success;
		char infoLog[1024];
		if(type != "PROGRAM"){
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success){
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: "<< type << "\n" << infoLog << "\n -----------------------------------" << std::endl;
			}
		}
		else{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success){
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -----------------------------------" << std::endl;
			}
		}
	}
};

#endif