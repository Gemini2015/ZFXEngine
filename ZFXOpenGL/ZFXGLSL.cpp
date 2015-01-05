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
	for (int i = 0; i < MAX_SHADER_NUM; i++)
	{
		m_Shader[i] = NULL;
	}
	m_nShaderNum = 0;
	m_nCurrentFShader = -1;
	m_nCurrentFShader = -1;
	m_CurrentProgram = 0;
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

HRESULT GLSLManager::CreateShader(const void* pData, GLenum type, bool bLoadFromFile, UINT *pID)
{
	if (pData == NULL)
		return E_INVALIDARG;
	if (pID == NULL)
		return E_INVALIDARG;
	(*pID) = UINT_MAX;

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

/*
	若 vshader 和 fshader 组成的program已存在，则返回program对象
	否则创建新的program对象
*/
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
	{
		// disable shader
		glUseProgram(0);
		m_CurrentProgram = 0;
		m_nCurrentVShader = -1;
		m_nCurrentFShader = -1;
		return ZFX_OK;
	}

	if (nVShader == m_nCurrentVShader &&
		nFShader == m_nCurrentFShader && m_bProgramCreated)
		return ZFX_OK;

	GLuint program = 0;
	HRESULT hr = E_FAIL;
	hr = CreateProgram(nVShader, nFShader, &program);
	if (FAILED(hr))
	{
		return hr;
	}
	m_bProgramCreated = true;
	glUseProgram(program);
	m_nCurrentFShader = nVShader;
	m_nCurrentFShader = nFShader;
	m_CurrentProgram = program;

	CollectConstant(program);

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

HRESULT GLSLManager::ActivateShader(UINT id, GLenum type)
{
	if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER)
		return E_INVALIDARG;

	if (id >= m_nShaderNum)
		return E_INVALIDARG;

	if (type == GL_VERTEX_SHADER && m_nCurrentVShader == id)
		return ZFX_OK;
	if (type == GL_FRAGMENT_SHADER && m_nCurrentFShader == id)
		return ZFX_OK;

	GLSLShader* pShader = m_Shader[id];
	if (type == GL_VERTEX_SHADER)
	{
		m_nCurrentVShader = id;
	}
	else
	{
		m_nCurrentFShader = id;
	}

	GLuint program;
	CreateProgram(m_nCurrentVShader, m_nCurrentFShader, &program);
	glUseProgram(program);
	m_CurrentProgram = program;

	CollectConstant(program);

	return ZFX_OK;
}

HRESULT GLSLManager::CollectConstant(GLuint program)
{
	if (!glIsProgram(program))
	{
		return ZFX_INVALIDPARAM;
	}

	m_ConstantList.clear();

	GLint num = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num);

	for (int i = 0; i < num; i++)
	{
		char buf[MAX_PATH] = {0};
		GLsizei length = 0;
		GLsizei size = 0;
		GLenum type = 0;
		glGetActiveUniform(program, i, MAX_PATH, &length, &size, &type, buf);

		if (length > 0)
		{

			GLint location = glGetUniformLocation(program, buf);
			if (location >= 0)
			{
				GLSLConstant constant;
				constant.name.assign(buf);
				constant.size = size;
				constant.type = type;
				constant.location = location;

				m_ConstantList[constant.name] = constant;
			}
		}
	}
}

HRESULT GLSLManager::SetNamedConstant(std::string name, float v)
{
	if (!glIsProgram(m_CurrentProgram))
		return E_FAIL;
	ConstantList_Map::iterator it = m_ConstantList.find(name);
	if (it == m_ConstantList.end())
	{
		return E_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT)
	{
		glUniform1f(constant.location, v);
	}
}

HRESULT GLSLManager::SetNamedConstant(std::string name, ZFXMatrix m)
{
	if (!glIsProgram(m_CurrentProgram))
		return E_FAIL;
	ConstantList_Map::iterator it = m_ConstantList.find(name);
	if (it == m_ConstantList.end())
	{
		return E_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT_MAT4)
	{
		glUniformMatrix4fv(constant.location, 1, GL_FALSE, &m._11);
	}
}

HRESULT GLSLManager::SetNamedConstant(std::string name, int i)
{
	if (!glIsProgram(m_CurrentProgram))
		return E_FAIL;
	ConstantList_Map::iterator it = m_ConstantList.find(name);
	if (it == m_ConstantList.end())
	{
		return E_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_INT)
	{
		glUniform1i(constant.location, i);
	}
}




