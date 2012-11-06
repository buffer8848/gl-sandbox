#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class GLShader
{
public:
	GLShader(std::string vertFile, std::string fragFile, std::string geomFile);
	GLShader(std::string vertFile, std::string fragFile);
	virtual ~GLShader(void);

	void Compile();	
	bool  initGLShaders(); //Sets up our glsl objects

	GLuint getVert();
	GLuint getFrag();
	GLuint getGeom();
private:
	const GLchar*	m_vertString;	
	const GLchar*	m_fragString;	
	const GLchar*	m_geomString;
	int		m_vertSize;
	int		m_fragSize;
	int		m_geomSize;
	GLuint  m_vertObj;	
	GLuint  m_fragObj;	
	GLuint  m_geomObj;

	GLchar* loadString(std::ifstream& in, int* length); //Gets a char* from input stream, stores the array size in *length
};

