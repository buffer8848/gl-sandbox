#include "GLRenderer.h"
#include <iostream>
GLRenderer::GLRenderer(void)
{
	
}
GLRenderer::~GLRenderer(void)
{
}


bool GLRenderer::CreateGLContext(HDC *pDC)
{
	PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                        //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                        //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
 
	int nPixelFormat = ChoosePixelFormat(*pDC, &pfd);
 
	if (nPixelFormat == 0) return false;
 
	BOOL bResult = SetPixelFormat (*pDC, nPixelFormat, &pfd);
 
	if (!bResult){
		std::cout << "Warning: could not set pixel format!\n";
		return false; 
	}
 
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

void GLRenderer::PrepareScene()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
 
	m_pShader = new GLShader("shaders/test.vert", "shaders/test.frag");
	
	if(glIsShader(m_pShader->getVert()) == GL_FALSE)
	{
		std::cout << "Warning: Vertex shader " << (*m_pShader).getVert() << "  is invalid!\n";
	}
	m_pShader->Compile();
	
	
	m_program = glCreateProgram();
	glAttachShader(m_program, m_pShader->getVert());
	glAttachShader(m_program, m_pShader->getFrag());
 
	glBindAttribLocation(m_program, 0, "in_Position");
	glBindAttribLocation(m_program, 1, "in_Color");
 
	glLinkProgram(m_program);
	glDetachShader(m_program, m_pShader->getVert());
	glDetachShader(m_program, m_pShader->getFrag());

	int bDidLink;
	glGetProgramiv(m_program, GL_LINK_STATUS, &bDidLink);
	if(bDidLink == GL_FALSE)
	{
		std::cout << "Warning(" << glGetError() << "): Shader program failed to link! Writing log file..\n";
		GLsizei bufSize; 
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &bufSize);
		GLsizei length;
		char* log = new char[bufSize];
		glGetProgramInfoLog(m_program, bufSize, &length, log);
		std::ofstream logStream;
		logStream.open ("glpglog.txt");
		logStream << log;
		logStream.close();
	}

	glUseProgram(m_program);

	SetData();
}

void GLRenderer::SetData()
{
	// First simple object
	float* vert = new float[9];	// vertex array
	float* col  = new float[9];	// color array
 
	vert[0] =-0.3; vert[1] = 0.5; vert[2] =-1.0;
	vert[3] =-0.8; vert[4] =-0.5; vert[5] =-1.0;
	vert[6] = 0.2; vert[7] =-0.5; vert[8]= -1.0;
 
	col[0] = 1.0; col[1] = 0.0; col[2] = 0.0;
	col[3] = 0.0; col[4] = 1.0; col[5] = 0.0;
	col[6] = 0.0; col[7] = 0.0; col[8] = 1.0;
 
	// Second simple object
	float* vert2 = new float[9];	// vertex array
 
	vert2[0] =-0.2; vert2[1] = 0.5; vert2[2] =-1.0;
	vert2[3] = 0.3; vert2[4] =-0.5; vert2[5] =-1.0;
	vert2[6] = 0.8; vert2[7] = 0.5; vert2[8]= -1.0;
 
	// Two VAOs allocation
	glGenVertexArrays(2, &m_vaoID[0]);
 
	// First VAO setup
	glBindVertexArray(m_vaoID[0]);
 
	glGenBuffers(2, m_vboID);
 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), col, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
 
	// Second VAO setup	
	glBindVertexArray(m_vaoID[1]);
 
	glGenBuffers(1, &m_vboID[2]);
 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vert2, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
 
	glBindVertexArray(0);
 
	delete [] vert;
	delete [] vert2;
	delete [] col;
}

void GLRenderer::Reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
}

void GLRenderer::DrawScene(HDC *pDC)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	glBindVertexArray(m_vaoID[0]);		// select first VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw first object
 
	glBindVertexArray(m_vaoID[1]);		// select second VAO
	glVertexAttrib3f((GLuint)1, 1.0, 0.0, 0.0); // set constant color attribute
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw second object
 
	glBindVertexArray(0);
 
	SwapBuffers(*pDC);
}