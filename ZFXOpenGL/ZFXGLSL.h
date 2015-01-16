#ifndef _ZFXGLSL_H_
#define _ZFXGLSL_H_

#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "ZFXGLPrerequisites.h"
#include "ZFXShaderManager.h"

#include <string>
#include <vector>
#include <map>


typedef struct GLTYPE_ZFXTYPE_TYPE
{
	ZFXDATATYPE zfx_type;
	GLenum gl_type;
}GLTYPE_ZFXTYPE;


class GLSLShaderObject : public ShaderObject
{
private:

	static UINT sShaderUUID;

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
	ZFXDATATYPE type;
	GLuint location;
} GLSLConstant;

enum AUTOCONSTANT_ID
{
	ACT_MODELVIEW_MATRIX = 0,
	ACT_MODELVIEWPROJ_MATRIX,
	ACT_NORMAL_MATRIX,

	// Light stuff
	ACT_LIGHT_AMBIENT,
	ACT_LIGHT_DIFFUSE,
	ACT_LIGHT_SPECULAR,
	ACT_LIGHT_DIRECTION,
	ACT_LIGHT_POSITION,
	//ACT_LIGHT_SHININESS,
	ACT_LIGHT_STRENGTH,
	ACT_LIGHT_SPOT_CUTOFF,
	ACT_LIGHT_SPOT_EXPONENT,
	ACT_LIGHT_CONSTANT_ATTENUATION,
	ACT_LIGHT_LINEAR_ATTENUATION,
	ACT_LIGHT_QUAD_ATTENUATION,

	// material
	ACT_MATERIAL_AMBIENT,
	ACT_MATERIAL_DIFFUSE,
	ACT_MATERIAL_SPECULAR,
	ACT_MATERIAL_EMISSION,
	ACT_MATERIAL_SHININESS,

	MAX_AUTOCONSTANT_ID,
};

typedef struct GLSLAutoConstant_Type
{
	AUTOCONSTANT_ID id;
	std::string name;
	ZFXDATATYPE type;
	GLSLAutoConstant_Type()
	{
	}
	GLSLAutoConstant_Type(AUTOCONSTANT_ID id, std::string name, ZFXDATATYPE type)
	{
		this->id = id;
		this->name.assign(name);
		this->type = type;
	}
}GLSLAutoConstant;

class GLSLShaderManager : public IShaderManager
{
	typedef std::map<UINT, GLSLShaderObject*> GLSLSHADER_MAP;
	GLSLSHADER_MAP m_ShaderObjectMap;

	typedef std::map<UINT64, GLSLProgram*> GLSLPROGRAM_MAP;
	GLSLPROGRAM_MAP m_ProgramMap;

	typedef std::map<std::string, GLSLConstant> GLSLCONSTANT_MAP;
	GLSLCONSTANT_MAP m_ConstantMap;

	typedef std::map<GLenum, ZFXDATATYPE> GLTYPE_ZFXTYPE_MAP;
	GLTYPE_ZFXTYPE_MAP m_DataTypeMap;

	GLSLProgram* m_ActiveProgram;

public:
	

	static GLSLAutoConstant sGLSLAutoConstantDict[MAX_AUTOCONSTANT_ID];

	GLSLShaderManager(ZFXOpenGL* pOpenGL);

	virtual ShaderObject* CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile) override;

	virtual HRESULT BindShader(ShaderObject* obj) override;

	virtual HRESULT UnBindShader(ZFXSHADERTYPE type) override;

	virtual ShaderObject* GetActiveShader(ZFXSHADERTYPE type) override;

	virtual HRESULT EnableShader(bool bEnable) override;

	GLSLProgram* GetActiveProgram();
	
	GLSLProgram* LinkProgram();

	HRESULT CollectConstant(GLuint program);

	HRESULT UpdateAutoConstant();

	virtual HRESULT SetNamedConstant(std::string name, bool val) override;

	virtual HRESULT SetNamedConstant(std::string name, int val) override;

	virtual HRESULT SetNamedConstant(std::string name, float val) override;

	virtual HRESULT SetNamedConstant(std::string name, ZFXMatrix m) override;

	virtual HRESULT SetNamedConstant(std::string name, ZFXDATATYPE type, int count, void* data) override;

	HRESULT SetTextureSampler(int nTex);

	virtual ShaderObject* GetShaderByName(std::string file) override;

	virtual ShaderObject* GetShaderByID(UINT id) override;

};



#endif