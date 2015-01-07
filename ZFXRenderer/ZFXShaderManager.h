#ifndef _ZFX_SHADER_MANAGER_H_
#define _ZFX_SHADER_MANAGER_H_

#include <windows.h>
#include "ZFX.h"

#include <string>



class ShaderObject
{
protected:

	UINT m_uuid;
	// ����
	ZFXSHADERTYPE m_type;
	// Դ�����ļ�
	std::string m_file;
	// Դ����
	std::string m_Source;

	// �Ƿ��ѱ���
	bool m_bCompiled;
	// �Ƿ�������Դ��
	bool m_bLoaded;
	bool m_bLoadFromFile;
	
public:
	ShaderObject(ZFXSHADERTYPE type);
	
	ZFXSHADERTYPE GetType() const { return m_type; }
	bool IsLoaded() const { return m_bLoaded; }
	bool IsLoadFromFile() const { return m_bLoadFromFile; }
	bool IsCompiled() const { return m_bCompiled; }
	UINT GetID() const { return m_uuid; }
	virtual HRESULT LoadSource(std::string str, bool bLoadFromFile = true);
	virtual HRESULT Compile(void) = 0;

};

class IShaderManager
{
protected:
	ShaderObject* m_ActiveVertexShader;
	ShaderObject* m_ActiveFragmentShader;
	
public:
	IShaderManager()
	{
		m_ActiveVertexShader = NULL;
		m_ActiveFragmentShader = NULL;
	}

	virtual ShaderObject* CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile) = 0;
	virtual HRESULT BindShader(ShaderObject* obj) = 0;
	virtual HRESULT UnBindShader(ZFXSHADERTYPE type) = 0;
	virtual ShaderObject* GetActiveShader(ZFXSHADERTYPE type) = 0;
	virtual HRESULT EnableShader(bool bEnable) = 0;

	virtual HRESULT SetNamedConstant(std::string name, bool val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, int val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, float val) = 0;
	virtual HRESULT SetNamedConstant(std::string name, ZFXMatrix m) = 0;
};




#endif