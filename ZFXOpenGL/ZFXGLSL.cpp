#include "ZFXGLSL.h"

/************************************************************************/
/*
GLSL shader wrap
*/
/************************************************************************/
GLSLShader::GLSLShader()
{
	m_bCompiled = false;
	m_shader = 0;
	m_type = 0;
	m_strSource = "";
}

HRESULT GLSLShader::CreateShader(GLenum type, std::string source)
{
	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
		return E_INVALIDARG;

	m_type = type;
	m_strSource = source;
	m_bCompiled = false;

	// create shader
	m_shader = glCreateShader(m_type);
	if (m_shader)
		return E_FAIL;

	const char* str = m_strSource.c_str();
	glShaderSource(m_shader, 1, (GLchar**)&(str), NULL);

	return Compile();
}

HRESULT GLSLShader::Compile()
{
	// compile shader
	glCompileShader(m_shader);
	GLint statue = GL_FALSE;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &statue);
	if (statue != GL_TRUE)
	{
		// get compile message
		glDeleteShader(m_shader);
		return E_FAIL;
	}
	m_bCompiled = true;
	return ZFX_OK;
}

GLSLShader::~GLSLShader()
{
	glDeleteShader(m_shader);
}

GLSLProgram::GLSLProgram()
{
	m_bLinked = false;
	m_nVShader = 0;
	m_nFShader = 0;
	m_program = 0;
}

GLSLProgram::~GLSLProgram()
{
	
	glDeleteProgram(m_program);
}

HRESULT GLSLProgram::CreateProgram(GLuint vshader /*= 0*/, GLuint fshader /*= 0*/)
{
	if (vshader == 0 && fshader == 0)
		return E_INVALIDARG;

	m_bLinked = false;

	// create program
	m_program = glCreateProgram();
	if (m_program == 0)
		return E_FAIL;

	if (vshader != 0)
		glAttachShader(m_program, vshader);
	if (fshader != 0)
		glAttachShader(m_program, fshader);

	glLinkProgram(m_program);

	GLint res = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &res);
	if (res != GL_TRUE)
	{
		// link program error
		return E_FAIL;
	}

	m_nVShader = vshader;
	m_nFShader = fshader;
	m_bLinked = true;
	return ZFX_OK;
}

GLSLManager::GLSLManager()
{
	memset(m_Shader, 0, sizeof(GLSLShader) * MAX_SHADER_NUM);
	m_nShaderNum = 0;
	m_nCurrentFShader = -1;
	m_nCurrentFShader = -1;
	m_vProgram.clear();
}

GLSLManager::~GLSLManager()
{
	for (int i = 0; i < m_nShaderNum; i++)
	{
		if (m_Shader[i])
		{
			delete m_Shader[i];
			m_Shader[i] = NULL;
		}
	}
	for (int i = 0; i < m_vProgram.size(); i++)
	{
		if (m_vProgram[i])
		{
			delete m_vProgram[i];
			m_vProgram[i] = NULL;
		}
	}
	m_nShaderNum = 0;
	m_nCurrentVShader = -1;
	m_nCurrentFShader = -1;
}

HRESULT GLSLManager::CreateShader(const void* pData, GLenum type, bool bLoadFromFile, int *pID)
{
	(*pID) = -1;
	if (pData == NULL)
		return E_INVALIDARG;
	if (pID == NULL)
		return E_INVALIDARG;
	

	if (m_nShaderNum >= (MAX_SHADER_NUM - 1))
		return ZFX_OUTOFMEMORY;
	
	// load shader source string
	HANDLE hFile = NULL, hMap = NULL;
	DWORD *pShaderSource = NULL;
	// get shader source
	if (bLoadFromFile)
	{
		hFile = CreateFile((LPCTSTR)pData, GENERIC_READ,
			0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return ZFX_FILENOTFOUND;
		}

		hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		pShaderSource = (DWORD *)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	}
	else
	{
		pShaderSource = (DWORD *)pData;
	}
	std::string tempstr;
	tempstr.assign((const char*)pShaderSource);

	// create shader
	GLSLShader *pShader = new GLSLShader();
	if (pShader == NULL)
		return ZFX_OUTOFMEMORY;

	HRESULT hr = pShader->CreateShader(type, tempstr);
	if (FAILED(hr))
	{
		delete pShader;
		return hr;
	}
	
	if (bLoadFromFile)
	{
		UnmapViewOfFile(pShaderSource);
		CloseHandle(hMap);
		CloseHandle(hFile);
	}

	m_Shader[m_nShaderNum] = pShader;
	(*pID) = m_nShaderNum;
	m_nShaderNum++;
	return ZFX_OK;
}

HRESULT GLSLManager::CreateProgram(int nVShader /*= -1*/, int nFShader /*= -1*/, GLuint *program /*= NULL*/)
{
	if (nVShader == -1 && nFShader == -1)
		return E_INVALIDARG;

	GLuint vshader = 0;
	GLuint fshader = 0;
	if (nVShader < m_nShaderNum)
	{
		GLSLShader *pvshader = m_Shader[nVShader];
		if (pvshader != NULL && pvshader->m_type == GL_VERTEX_SHADER
			&& pvshader->m_bCompiled)
			vshader = pvshader->m_shader;
	}
	if (nFShader < m_nShaderNum)
	{
		GLSLShader *pfshader = m_Shader[nFShader];
		if (pfshader != NULL && pfshader->m_type == GL_FRAGMENT_SHADER
			&& pfshader->m_bCompiled)
			fshader = pfshader->m_shader;
	}

	int programID = FindProgram(vshader, fshader);
	if (programID != 0)
	{
		if (program)
		{
			(*program) = programID;
		}
		return ZFX_OK;
	}
	GLSLProgram* pProgram = new GLSLProgram();
	if (pProgram == NULL)
	{
		return ZFX_OUTOFMEMORY;
	}
	HRESULT hr = pProgram->CreateProgram(vshader, fshader);
	if (FAILED(hr))
	{
		delete pProgram;
		return hr;
	}
	m_vProgram.push_back(pProgram);
	
	if (program)
	{
		(*program) = pProgram->m_program;
	}
	return ZFX_OK;
}

HRESULT GLSLManager::UseShader(int nVShader /*= -1*/, int nFShader /*= -1*/)
{
	if (nVShader == -1 && nFShader == -1)
		return E_INVALIDARG;

	if (nVShader == m_nCurrentVShader &&
		nFShader == m_nCurrentFShader)
		return ZFX_OK;

	GLuint program = 0;
	HRESULT hr = E_FAIL;
	hr = CreateProgram(nVShader, nFShader, &program);
	if (FAILED(hr))
	{
		return hr;
	}
	
	glUseProgram(program);
	m_nCurrentFShader = nVShader;
	m_nCurrentFShader = nFShader;

	return ZFX_OK;
}

GLuint GLSLManager::FindProgram(GLuint vshader /*= 0*/, GLuint fshader /*= 0*/)
{
	std::vector<GLSLProgram*>::iterator it = m_vProgram.begin();
	GLuint program = 0;
	for (; it != m_vProgram.end(); it++)
	{
		GLSLProgram* pProgram = (*it);
		if (pProgram && pProgram->m_nVShader == vshader
			&& pProgram->m_nFShader == fshader
			&& pProgram->m_bLinked)
		{
			program = pProgram->m_program;
			break;
		}
	}
	return program;
}




