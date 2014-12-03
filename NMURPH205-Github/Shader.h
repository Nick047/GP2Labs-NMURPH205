#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>


//Used for mac compatability
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif WIN32

#include <SDL_opengl.h>

#endif


#include <iostream>
#include <string>
#include <fstream>


enum SHADER_TYPE
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

GLuint loadShaderFromFile(const std::string&filename, SHADER_TYPE shaderType);
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType);	//If this function fails then the value returned will be 0

bool checkForLinkErrors(GLuint program);
bool checkForCompileErrors(GLuint shaderProgram);

#endif





