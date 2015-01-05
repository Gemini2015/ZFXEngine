#ifndef _ZFXGLSL_H_
#define _ZFXGLSL_H_

#include <windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "ZFXRenderDevice.h"

#include <string>
#include <vector>
#include <map>

#define MAX_SHADER_NUM (MAX_SHADER * 2)


class GLSLShader
{
public:
	GLuint m_shader;
	GLenum m_type;
	bool m_bCompiled;
	std::string m_strSource;

	GLSLShader();
	~GLSLShader();
	HRESULT CreateShader(GLenum type, std::string source);
	HRESULT Compile();
};

class GLSLProgram
{
public:
	GLuint m_nVShader;
	GLuint m_nFShader;
	GLuint m_program;
	bool m_bLinked;

	GLSLProgram();
	~GLSLProgram();
	HRESULT CreateProgram(GLuint vshader = 0, GLuint fshader = 0);
};

typedef struct GLSLConstant_Type
{
	std::string name;
	GLsizei size;
	GLenum type;
	GLuint location;
} GLSLConstant;

class GLSLManager
{
public:
	GLSLShader* m_Shader[MAX_SHADER_NUM];
	int m_nShaderNum;
	std::vector<GLSLProgram*> m_vProgram;

	int m_nCurrentVShader;
	int m_nCurrentFShader;
	GLuint m_CurrentProgram;
	bool m_bProgramCreated;

	typedef std::map<std::string, GLSLConstant> ConstantList_Map;
	ConstantList_Map m_ConstantList;

	GLSLManager();
	~GLSLManager();

	HRESULT CreateShader(const void* pData, GLenum type, bool bLoadFromFile, UINT *pID);
	HRESULT CreateProgram(int nVShader = -1, int nFShader = -1, GLuint *program = NULL);
	HRESULT UseShader(int nVShader = -1, int nFShader = -1);
	HRESULT ActivateShader(UINT id, GLenum type);
	GLuint GetActiveProgram()
	{
		return m_CurrentProgram;
	}
	GLuint FindProgram(GLuint vshader = 0, GLuint fshader = 0);
	

	HRESULT CollectConstant(GLuint program);
	HRESULT SetNamedConstant(std::string name, int i);
	HRESULT SetNamedConstant(std::string name, float v);
	HRESULT SetNamedConstant(std::string name, ZFXMatrix m);
};

#endif