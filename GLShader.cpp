#include "GLShader.h"
#include <direct.h>
GLShader::GLShader( std::string vertFile, std::string fragFile, std::string geomFile = ""  )
{
	std::cout << "[Constructor]";
	getVert();
	std::ifstream vert_ifs(vertFile, std::ios::in);
	m_vertString = loadString(vert_ifs, &m_vertSize);
	std::cout << m_vertSize;
	if(m_vertString == NULL)
	{
		throw(0);
	}

	std::ifstream frag_ifs(fragFile.c_str(), std::ifstream::in);
	m_fragString = loadString(frag_ifs, &m_fragSize);
	if(m_fragString == NULL)
	{
		throw(1);
	}

	if(geomFile != "")
	{
		std::ifstream geom_ifs(geomFile.c_str(), std::ifstream::in);
		m_geomString = loadString(geom_ifs, &m_geomSize);
		if(m_geomString == NULL)
		{
			throw(2);
		}
	}else{
		m_geomSize = 0;
		m_geomObj = 0;
	}
}


GLShader::GLShader(std::string vertFile, std::string fragFile)
{
	GLShader(vertFile, fragFile, "");
}

GLShader::~GLShader(void)
{
	//glDeleteShader(m_vertObj);
	//glDeleteShader(m_fragObj);
	//glDeleteShader(m_geomObj);

	//delete [] m_vertString;
	//delete [] m_fragString;
	//delete [] m_geomString;
}

GLchar* GLShader::loadString(std::ifstream& in, int* length)
{
	std::cout << "in.is_open() = " << in.is_open() << std::endl;
	std::cout << "in.good() = " << in.good() << std::endl;
	if(!in.good()){
		std::cout << "failbit = " << in.failbit << std::endl;
		return NULL;
	}
	char* buf;

	in.seekg (0, std::ios::end);
	*length = in.tellg();
	in.seekg (0, std::ios::beg);

	std::cout << "Length = " << *length << std::endl;

	buf = new GLchar[*length];
	in.read(buf, *length);

	return buf;
}

bool GLShader::initGLShaders()
{
	this->m_vertObj = glCreateShader(GL_VERTEX_SHADER);
	if(!m_vertObj) return false;

	std::cout << "[initGLShaders]";
	getVert();

	m_fragObj = glCreateShader(GL_FRAGMENT_SHADER);
	if(!m_fragObj) return false;

	std::cout << "[initGLShaders frag]";
	getFrag();

	if(m_geomSize > 0)
	{
		m_geomObj = glCreateShader(GL_GEOMETRY_SHADER);
		if(!m_geomObj) return false;
	}

	return true;
}

void GLShader::Compile()
{
	glShaderSource(m_vertObj, 1, &m_vertString, &m_vertSize);
	glCompileShader(m_vertObj);

	int bDidCompile;
	glGetShaderiv(m_vertObj, GL_COMPILE_STATUS, &bDidCompile);
	if(bDidCompile == GL_FALSE)
	{
		std::cout << "Warning: Vertex shader not compiled!\n";
	}


	glShaderSource(m_fragObj, 1, &m_fragString, &m_fragSize);
	glCompileShader(m_fragObj);

	glGetShaderiv(m_vertObj, GL_COMPILE_STATUS, &bDidCompile);
	if(bDidCompile == GL_FALSE)
	{
		std::cout << "Warning: Fragment shader not compiled!\n";
	}

	if(m_geomSize > 0)
	{
		glShaderSource(m_geomObj, 1, &m_geomString, &m_geomSize);
		glCompileShader(m_geomObj);
		glGetShaderiv(m_vertObj, GL_COMPILE_STATUS, &bDidCompile);
		if(bDidCompile == GL_FALSE)
		{
			std::cout << "Warning: Geometry shader not compiled!\n";
		}
	}
}

GLuint GLShader::getVert()
{ 
	std::cout << "getVert() returns " << m_vertObj << std::endl; 
	return m_vertObj; 
}

GLuint GLShader::getFrag()
{ 
	std::cout << "getFrag() returns " << m_fragObj << std::endl; 
	return m_fragObj; 
}
GLuint GLShader::getGeom()
{	
	std::cout << "getFrag() returns " << m_geomObj << std::endl; 
	return m_geomObj; 
}