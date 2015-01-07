#ifndef _ZFXGLSL_H_
#define _ZFXGLSL_H_

#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "ZFXRenderDevice.h"
#include "ZFXShaderManager.h"

#include <string>
#include <vector>
#include <map>

#define MAX_SHADER_NUM (MAX_SHADER * 2)

class ZFXOpenGL;

class GLSLShaderObject : public ShaderObject
{
private:

	static UINT sVertexShaderUUID;
	static UINT sFragmentShaderUUID;

	GLuint m_ShaderObject;
	
public:
	GLSLShaderObject(ZFXSHADERTYPE type);
	HRESULT Compile(void);

	GLuint GetShaderObject() const { return m_ShaderObject; }
};

class GLSLProgram
{
public:
	UINT64 m_uuid;
	GLSLShaderObject* m_VShader;
	GLSLShaderObject* m_FShader;

	GLuint m_program;
	bool m_bLinked;

	GLSLProgram();
	~GLSLProgram();
	HRESULT AttachShader(GLSLShaderObject *shader);
	HRESULT Link();
};

typedef struct GLSLConstant_Type
{
	std::string name;
	GLsizei size;
	GLenum type;
	GLuint location;
} GLSLConstant;

class GLSLShaderManager : public IShaderManager
{
	typedef std::map<UINT, GLSLShaderObject> GLSLSHADER_MAP;
	GLSLSHADER_MAP m_ShaderObjectMap;

	typedef std::map<UINT64, GLSLProgram> GLSLPROGRAM_MAP;
	GLSLPROGRAM_MAP m_ProgramMap;

	typedef std::map<std::string, GLSLConstant> GLSLCONSTANT_MAP;
	GLSLCONSTANT_MAP m_ConstantMap;

	GLSLProgram* m_ActiveProgram;

public:
	GLSLShaderManager(ZFXOpenGL* pOpenGL);

	virtual ShaderObject* CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile) override;

	virtual HRESULT BindShader(ShaderObject* obj) override;

	virtual HRESULT UnBindShader(ZFXSHADERTYPE type) override;

	virtual ShaderObject* GetActiveShader(ZFXSHADERTYPE type) override;

	virtual HRESULT EnableShader(bool bEnable) override;

	GLSLProgram* GetActiveProgram();
	
	GLSLProgram* LinkProgram();

	HRESULT CollectConstant(GLuint program);

	virtual HRESULT SetNamedConstant(std::string name, bool val) override;

	virtual HRESULT SetNamedConstant(std::string name, int val) override;

	virtual HRESULT SetNamedConstant(std::string name, float val) override;

	virtual HRESULT SetNamedConstant(std::string name, ZFXMatrix m) override;
};



#endif