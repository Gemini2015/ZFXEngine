#ifndef _ZFX_SHADER_MANAGER_H_
#define _ZFX_SHADER_MANAGER_H_

#include <windows.h>
#include <string>

#include "ZFXPrerequisites.h"

class ShaderObject
{
protected:

	UINT m_uuid;
	// 类型
	ZFXSHADERTYPE m_type;
	// 源代码文件
	std::string m_file;
	// 源代码
	std::string m_Source;

	// 是否已编译
	bool m_bCompiled;
	// 是否已载入源码
	bool m_bLoaded;
	bool m_bLoadFromFile;
	
public:
	ShaderObject(ZFXSHADERTYPE type);
	
	ZFXSHADERTYPE GetType() const { return m_type; }
	bool IsLoaded() const { return m_bLoaded; }
	bool IsLoadFromFile() const { return m_bLoadFromFile; }
	bool IsCompiled() const { return m_bCompiled; }
	UINT GetID() const { return m_uuid; }
	std::string GetFileName() const { return m_file; }
	virtual HRESULT LoadSource(std::string str, bool bLoadFromFile = true);
	virtual HRESULT Compile(void) = 0;

};

class IShaderManager
{
protected:
	ZFXRenderDevice* m_pRenderDevice;
	ShaderObject* m_ActiveVertexShader;
	ShaderObject* m_ActiveFragmentShader;

	bool m_bCanUseShader;
	bool m_bUseShader;
	
public:
	IShaderManager(ZFXRenderDevice* pRenderDevice)
	{
		m_pRenderDevice = pRenderDevice;
		m_ActiveVertexShader = NULL;
		m_ActiveFragmentShader = NULL;
	}

	virtual ShaderObject* CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile) = 0;
	virtual HRESULT BindShader(ShaderObject* obj) = 0;
	virtual HRESULT UnBindShader(ZFXSHADERTYPE type) = 0;
	virtual ShaderObject* GetActiveShader(ZFXSHADERTYPE type) = 0;
	/*
		Step.1 Bind Shader
		Step.2 Enable Shader
	*/
	virtual HRESULT EnableShader(bool bEnable) = 0;
	virtual bool IsCanUseShader() const { return m_bCanUseShader; };
	virtual bool IsUseShader() const { return m_bUseShader; };

	virtual ShaderObject* GetShaderByName(std::string file) = 0;
	virtual ShaderObject* GetShaderByID(UINT id) = 0;

	virtual HRESULT SetNamedConstant(std::string name, ZFXDATATYPE type, int count, void* data) = 0;
	virtual HRESULT SetNamedConstant(std::string name, bool val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, int val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, float val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, ZFXMatrix m) = 0;
};




#endif