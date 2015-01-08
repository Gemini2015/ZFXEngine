#pragma once

#include "ZFXShaderManager.h"
class ZFXD3D;

class HLSLShaderManager :
	public IShaderManager
{
public:
	HLSLShaderManager(ZFXD3D *pD3D);
	~HLSLShaderManager();

	virtual ShaderObject* CreateShader(const void* pData, ZFXSHADERTYPE type, bool bLoadFromFile) override;

	virtual HRESULT BindShader(ShaderObject* obj) override;

	virtual HRESULT UnBindShader(ZFXSHADERTYPE type) override;

	virtual ShaderObject* GetActiveShader(ZFXSHADERTYPE type) override;

	virtual HRESULT EnableShader(bool bEnable) override;

	virtual HRESULT SetNamedConstant(std::string name, bool val) override;

	virtual HRESULT SetNamedConstant(std::string name, int val) override;

	virtual HRESULT SetNamedConstant(std::string name, float val) override;

	virtual HRESULT SetNamedConstant(std::string name, ZFXMatrix m) override;

	virtual HRESULT SetNamedConstant(std::string name, ZFXDATATYPE type, int count, void* data) override;

};

