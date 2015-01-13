#include "ZFXGLSL.h"

/************************************************************************/
/*
GLSL program wrap
*/
/************************************************************************/

GLSLProgram::GLSLProgram()
{
	m_uuid = 0;
	m_VShader = NULL;
	m_FShader = NULL;

	m_program = 0;
	m_bLinked = false;
}

GLSLProgram::~GLSLProgram()
{
	if(glIsProgram(m_program))
		glDeleteProgram(m_program);
}

HRESULT GLSLProgram::Link()
{
	if (m_VShader == NULL && m_FShader == NULL)
		return ZFX_FAIL;

	m_bLinked = false;

	m_program = glCreateProgram();
	if (m_program == 0)
		return ZFX_FAIL;

	m_uuid = 0;
	if (m_VShader != NULL && m_VShader->IsCompiled())
	{
		glAttachShader(m_program, m_VShader->GetShaderObject());
		m_uuid |= m_VShader->GetID() & 0xffff;
	}
	if (m_FShader != NULL && m_FShader->IsCompiled())
	{
		glAttachShader(m_program, m_FShader->GetShaderObject());
		m_uuid |= (m_FShader->GetID() & 0xffff) << 16;
	}

	glLinkProgram(m_program);

	GLint res = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &res);
	if (res != GL_TRUE)
	{
		return ZFX_FAIL;
	}

	m_bLinked = true;
	return ZFX_OK;
}

HRESULT GLSLProgram::AttachShader(GLSLShaderObject *shader)
{
	if (shader == NULL)
		return ZFX_INVALIDPARAM;

	HRESULT hr = ZFX_OK;
	switch (shader->GetType())
	{
	case SHT_VERTEX:
		m_VShader = shader;
		break;
	case SHT_PIXEL:
		m_FShader = shader;
		break;
	default:
		hr = ZFX_INVALIDPARAM;
		break;
	}
	return hr;
}

UINT GLSLShaderObject::sShaderUUID = 0;

GLSLShaderObject::GLSLShaderObject(ZFXSHADERTYPE type)
	:ShaderObject(type)
{
	m_ShaderObject = 0;
	m_uuid = sShaderUUID++;
}

HRESULT GLSLShaderObject::Compile(void)
{
	if (m_type != SHT_VERTEX && m_type != SHT_PIXEL)
		return ZFX_FAIL;

	if (!m_bLoaded)
		return ZFX_FAIL;

	m_bCompiled = false;

	// create shader
	GLenum type = m_type == SHT_VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	m_ShaderObject = glCreateShader(type);
	if (m_ShaderObject == 0)
		return E_FAIL;

	const char* str = m_Source.c_str();
	glShaderSource(m_ShaderObject, 1, (GLchar**)&(str), NULL);

	// compile shader
	glCompileShader(m_ShaderObject);
	GLint statue = GL_FALSE;
	glGetShaderiv(m_ShaderObject, GL_COMPILE_STATUS, &statue);
	if (statue != GL_TRUE)
	{
		// get compile message
		GLsizei len;
		glGetShaderiv(m_ShaderObject, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(m_ShaderObject, len, &len, log);

		std::cerr << "Shader compilation failed: " << log << std::endl;
		delete [] log;

		glDeleteShader(m_ShaderObject);
		return E_FAIL;
	}

	m_bCompiled = true;
	return ZFX_OK;
}

GLSLShaderManager::GLSLShaderManager(ZFXOpenGL *pOpenGL)
	: IShaderManager((ZFXRenderDevice*)pOpenGL)
{
	m_ShaderObjectMap.clear();
	m_ProgramMap.clear();
	m_ConstantMap.clear();
	m_DataTypeMap.clear();
	m_ActiveProgram = NULL;

	GLTYPE_ZFXTYPE TypeMapping[] = {
			{ DAT_BOOL, GL_BOOL, },
			{ DAT_INT, GL_INT, },
			{ DAT_FLOAT, GL_FLOAT, },
			{ DAT_FMAT4, GL_FLOAT_MAT4, },
	};

	for (int i = 0; i < sizeof(TypeMapping) / sizeof(GLTYPE_ZFXTYPE); i++)
	{
		m_DataTypeMap[TypeMapping[i].gl_type] = TypeMapping[i].zfx_type;
	}

	if (GLEW_ARB_vertex_program || GLEW_ARB_fragment_program)
	{
		m_bCanUseShader = true;
	}
}

ShaderObject* GLSLShaderManager::CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile)
{
	if (pData == NULL)
		return NULL;

	if (type != SHT_VERTEX && type != SHT_PIXEL)
		return NULL;

	GLSLShaderObject *shaderobject = new GLSLShaderObject(type);

	HRESULT hr = shaderobject->LoadSource(std::string((char*)pData), bLoadFromFile);

	if (FAILED(hr))
		return NULL;

	hr = shaderobject->Compile();

	if (FAILED(hr))
		return NULL;

	m_ShaderObjectMap[shaderobject->GetID()] = shaderobject;

	return shaderobject;
}

HRESULT GLSLShaderManager::BindShader(ShaderObject* obj)
{
	if (obj == NULL)
		return ZFX_INVALIDPARAM;

	HRESULT hr = ZFX_OK;

	switch (obj->GetType())
	{
	case SHT_VERTEX:
		m_ActiveVertexShader = obj;
		m_ActiveProgram = NULL;
		break;
	case SHT_PIXEL:
		m_ActiveFragmentShader = obj;
		m_ActiveProgram = NULL;
		break;
	default:
		hr = ZFX_INVALIDPARAM;
		break;
	}

	return hr;
}

HRESULT GLSLShaderManager::UnBindShader(ZFXSHADERTYPE type)
{
	HRESULT hr = ZFX_OK;
	switch (type)
	{
	case SHT_VERTEX:
		m_ActiveVertexShader = NULL;
		m_ActiveProgram = NULL;
		break;
	case SHT_PIXEL:
		m_ActiveFragmentShader = NULL;
		m_ActiveProgram = NULL;
		break;
	default:
		hr = ZFX_INVALIDPARAM;
		break;
	}

	return hr;
}

ShaderObject* GLSLShaderManager::GetActiveShader(ZFXSHADERTYPE type)
{
	ShaderObject *obj = NULL;
	switch (type)
	{
	case SHT_VERTEX:
		obj = m_ActiveVertexShader;
		break;
	case SHT_PIXEL:
		obj = m_ActiveFragmentShader;
		break;
	default:
		break;
	}
	return obj;
}

HRESULT GLSLShaderManager::EnableShader(bool bEnable)
{
	HRESULT hr = ZFX_OK;
	if (bEnable)
	{
		GLSLProgram* program = GetActiveProgram();
		if (program && program->m_bLinked)
		{
			glUseProgram(program->m_program);

			CollectConstant(program->m_program);

			m_bUseShader = true;
		}
		else
		{
			m_ActiveProgram = NULL;
			glUseProgram(0);
			hr = ZFX_FAIL;

			m_bUseShader = false;
		}
	}
	else
	{
		m_ActiveProgram = NULL;
		glUseProgram(0);

		m_bUseShader = false;
	}
	return hr;
}

GLSLProgram* GLSLShaderManager::GetActiveProgram()
{
	if (m_ActiveProgram)
		return m_ActiveProgram;

	UINT64 uuid = 0;
	if (m_ActiveVertexShader)
	{
		uuid = m_ActiveVertexShader->GetID() && 0xffff;
	}
	if (m_ActiveFragmentShader)
	{
		uuid |= (m_ActiveFragmentShader->GetID() && 0xffff) << 16;
	}

	GLSLPROGRAM_MAP::iterator it = m_ProgramMap.find(uuid);
	if (it == m_ProgramMap.end())
	{
		m_ActiveProgram = LinkProgram();
	}
	else
	{
		m_ActiveProgram = it->second;
	}

	return m_ActiveProgram;
}

GLSLProgram* GLSLShaderManager::LinkProgram()
{
	GLSLProgram *program = new GLSLProgram();
	if (program == NULL)
		return program;
	if (m_ActiveVertexShader)
		program->AttachShader((GLSLShaderObject*)m_ActiveVertexShader);
	if (m_ActiveFragmentShader)
		program->AttachShader((GLSLShaderObject*)m_ActiveFragmentShader);
	
	HRESULT hr = program->Link();

	if (FAILED(hr))
	{
		delete program;
		program = NULL;
	}
	else
	{
		m_ProgramMap[program->m_uuid] = program;
	}
	return program;
}

HRESULT GLSLShaderManager::CollectConstant(GLuint program)
{
	if (!glIsProgram(program))
	{
		return ZFX_INVALIDPARAM;
	}

	m_ConstantMap.clear();

	GLint num = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num);

	for (int i = 0; i < num; i++)
	{
		char buf[MAX_PATH] = { 0 };
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
				constant.type = m_DataTypeMap[type];
				constant.location = location;

				m_ConstantMap[constant.name] = constant;
			}
		}
	}

	return ZFX_OK;
}

HRESULT GLSLShaderManager::SetNamedConstant(std::string name, bool val)
{
	if (!m_ActiveProgram)
		return ZFX_FAIL;
	if (!glIsProgram(m_ActiveProgram->m_program))
		return ZFX_FAIL;
	GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(name);
	if (it == m_ConstantMap.end())
	{
		return ZFX_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT)
	{
		glUniform1i(constant.location, val);
	}
	return ZFX_OK;
}

HRESULT GLSLShaderManager::SetNamedConstant(std::string name, int val)
{
	if (!m_ActiveProgram)
		return ZFX_FAIL;
	if (!glIsProgram(m_ActiveProgram->m_program))
		return ZFX_FAIL;
	GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(name);
	if (it == m_ConstantMap.end())
	{
		return ZFX_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT)
	{
		glUniform1i(constant.location, val);
	}
	return ZFX_OK;
}

HRESULT GLSLShaderManager::SetNamedConstant(std::string name, float val)
{
	if (!m_ActiveProgram)
		return ZFX_FAIL;
	if (!glIsProgram(m_ActiveProgram->m_program))
		return ZFX_FAIL;
	GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(name);
	if (it == m_ConstantMap.end())
	{
		return ZFX_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT)
	{
		glUniform1f(constant.location, val);
	}
	return ZFX_OK;
}

HRESULT GLSLShaderManager::SetNamedConstant(std::string name, ZFXMatrix m)
{
	if (!m_ActiveProgram)
		return ZFX_FAIL;
	if (!glIsProgram(m_ActiveProgram->m_program))
		return ZFX_FAIL;
	GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(name);
	if (it == m_ConstantMap.end())
	{
		return ZFX_FAIL;
	}

	GLSLConstant constant = it->second;

	if (constant.type == GL_FLOAT)
	{
		glUniformMatrix4fv(constant.location, 1, GL_FALSE, &m._11);
	}
	return ZFX_OK;
}

HRESULT GLSLShaderManager::SetNamedConstant(std::string name, ZFXDATATYPE type, int count, void* data)
{
	if (!m_ActiveProgram)
		return ZFX_FAIL;
	if (!glIsProgram(m_ActiveProgram->m_program))
		return ZFX_FAIL;
	GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(name);
	if (it == m_ConstantMap.end())
	{
		return ZFX_FAIL;
	}
	GLSLConstant constant = it->second;
	if (constant.type != type)
		return ZFX_INVALIDPARAM;

	HRESULT hr = ZFX_OK;
	switch (type)
	{
	case DAT_BOOL:
		glUniform1iv(constant.location, 1, (GLint*)data);
		break;
	case DAT_INT:
		glUniform1iv(constant.location, 1, (GLint*)data);
		break;
	case DAT_FLOAT:
		glUniform1fv(constant.location, 1, (GLfloat*)data);
		break;
	case DAT_FMAT4:
		glUniformMatrix4fv(constant.location, 1, GL_FALSE, (GLfloat*)data);
		break;
	default:
		hr = ZFX_FAIL;
		break;
	}
	return hr;
}

ShaderObject* GLSLShaderManager::GetShaderByName(std::string file)
{
	GLSLSHADER_MAP::iterator it = m_ShaderObjectMap.begin();
	bool found = false;
	GLSLShaderObject* shaderobject = NULL;
	while (it != m_ShaderObjectMap.end())
	{
		shaderobject = it->second;
		if (shaderobject && shaderobject->GetFileName().compare(file) == 0)
		{
			found = true;
			break;
		}
		it++;
	}
	if (found)
	{
		return shaderobject;
	}
	else
	{
		return NULL;
	}
}

ShaderObject* GLSLShaderManager::GetShaderByID(UINT id)
{
	GLSLSHADER_MAP::iterator it = m_ShaderObjectMap.find(id);
	if (it == m_ShaderObjectMap.end())
	{
		return NULL;
	}
	return it->second;
}

GLSLAutoConstant GLSLShaderManager::sGLSLAutoConstantDict[] = {
	GLSLAutoConstant(ACT_WORLDVIEWPROJ_MATRIX, "modelviewproj_matrix", DAT_FMAT4),
	GLSLAutoConstant(ACT_NORMAL_MATRIX, "normal_matrix", DAT_FMAT4),
};

HRESULT GLSLShaderManager::UpdateAutoConstant()
{
	if (m_ConstantMap.empty())
		return ZFX_OK;

	for (int i = 0; i < MAX_AUTOCONSTANT_ID; i++)
	{
		GLSLCONSTANT_MAP::iterator it = m_ConstantMap.find(sGLSLAutoConstantDict[i].name);
		if (it != m_ConstantMap.end())
		{
			switch (sGLSLAutoConstantDict[i].id)
			{
			case ACT_WORLDVIEWPROJ_MATRIX:
			{
				float val[16];
				ZFXMatrix modelview;
				ZFXMatrix project;
				glGetFloatv(GL_MODELVIEW_MATRIX, val);
				modelview.SetColumnMajorMat(val);
				glGetFloatv(GL_PROJECTION_MATRIX, val);
				project.SetColumnMajorMat(val);
				ZFXMatrix mvp = (project * modelview).GetColumnMajorMat();
				glUniformMatrix4fv(it->second.location, 1, GL_FALSE, mvp.val);
			}
				break;
			case ACT_NORMAL_MATRIX:
			{
				ZFXMatrix modelview;
				glGetFloatv(GL_MODELVIEW_MATRIX, modelview.val);
				modelview.InverseAffine();
				modelview.Transpose();
				glUniformMatrix4fv(it->second.location, 1, GL_FALSE, modelview.val);
			}
				break;
			default:
				break;
			}
		}
	}
	return ZFX_OK;
}




