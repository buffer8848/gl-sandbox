#define GLEW_STATIC

#include <windows.h>
#include <wingdi.h>
#include <gl/glew.h>
#include <gl/wglew.h>
#pragma comment(lib, "glew32.lib")

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	bool CreateGLContext(HDC* pDC);			//Create an opengl rendering context
protected:
	void SetData();
private:
	HGLRC	m_hrc;					//OpenGL Rendering Context
	HWND*	p_hwnd;					//Pointer to window
};