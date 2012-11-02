#include "CGLRenderer.h"

CGLRenderer::CGLRenderer(void)
{
	
}

bool CGLRenderer::CreateGLContext(HDC *pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
 
	int nPixelFormat = ChoosePixelFormat(*pDC, &pfd);
 
	if (nPixelFormat == 0) return false;
 
	BOOL bResult = SetPixelFormat (*pDC, nPixelFormat, &pfd);
 
	if (!bResult) return false; 
 
	HGLRC tempContext = wglCreateContext(*pDC);
	wglMakeCurrent(*pDC, tempContext);
 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		return false;
	}
 
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
 
        if(wglewIsSupported("WGL_ARB_create_context") == 1)
        {
		m_hrc = wglCreateContextAttribsARB(*pDC,0, attribs);
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(*pDC, m_hrc);
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		m_hrc = tempContext;
	}
 
	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);
 
	//Or better yet, use the GL3 way to get the version number
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
 
	if (!m_hrc) return false;
 
	return true;
}