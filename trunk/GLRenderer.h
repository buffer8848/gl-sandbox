#pragma once
#define GLEW_STATIC
#pragma comment(lib, "glew32.lib")

#include <windows.h>
#include <wingdi.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#include "GLShader.h"

class GLRenderer
{
public:
	GLRenderer(void);
	virtual ~GLRenderer(void);
	bool CreateGLContext(HDC* pDC);			//Create an opengl rendering context
	void PrepareScene();
	void Reshape(int w, int h);
	void DrawScene(HDC *pDC);
protected:
	void SetData();
private:
	HGLRC	m_hrc;					//OpenGL Rendering Context
	HWND*	p_hwnd;					//Pointer to window
	GLShader* m_pShader;
	GLuint m_program;
	GLuint m_vaoID[2];			// two vertex array objects, one for each drawn object
	GLuint m_vboID[3];			// three VBOs
};