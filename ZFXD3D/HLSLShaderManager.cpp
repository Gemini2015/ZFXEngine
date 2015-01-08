#include "HLSLShaderManager.h"


HLSLShaderManager::HLSLShaderManager(ZFXD3D* pD3D)
	:IShaderManager((ZFXRenderDevice*)pD3D)
{
}


HLSLShaderManager::~HLSLShaderManager()
{
}

ShaderObject* HLSLShaderManager::CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::BindShader(ShaderObject* obj)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::UnBindShader(ZFXSHADERTYPE type)
{
	throw std::logic_error("The method or operation is not implemented.");
}

ShaderObject* HLSLShaderManager::GetActiveShader(ZFXSHADERTYPE type)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::EnableShader(bool bEnable)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::SetNamedConstant(std::string name, bool val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::SetNamedConstant(std::string name, int val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::SetNamedConstant(std::string name, float val)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::SetNamedConstant(std::string name, ZFXMatrix m)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT HLSLShaderManager::SetNamedConstant(std::string name, ZFXDATATYPE type, int count, void* data)
{
	throw std::logic_error("The method or operation is not implemented.");
}

ShaderObject* HLSLShaderManager::GetShaderByName(std::string file)
{
	throw std::logic_error("The method or operation is not implemented.");
}

ShaderObject* HLSLShaderManager::GetShaderByID(UINT id)
{
	throw std::logic_error("The method or operation is not implemented.");
}
