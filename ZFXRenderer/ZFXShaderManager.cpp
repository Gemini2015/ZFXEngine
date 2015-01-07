#include "ZFXShaderManager.h"

ShaderObject::ShaderObject(ZFXSHADERTYPE type)
{
	m_uuid = 0;
	m_type = type;

	m_file = "";
	m_Source = "";

	m_bLoaded = false;
	m_bCompiled = false;
	m_bLoadFromFile = false;
}

HRESULT ShaderObject::LoadSource(std::string str, bool bLoadFromFile)
{
	if (str.empty())
		return ZFX_INVALIDPARAM;

	// load shader source string
	HANDLE hFile = NULL, hMap = NULL;
	DWORD *pShaderSource = NULL;
	// get shader source
	if (bLoadFromFile)
	{
		hFile = CreateFile((LPCTSTR)str.c_str(), GENERIC_READ,
			0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return ZFX_FILENOTFOUND;
		}

		hMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		pShaderSource = (DWORD *)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

		m_Source.assign((const char*)pShaderSource);
		m_file = str;
	}
	else
	{
		m_Source = str;
	}
	m_bLoadFromFile = bLoadFromFile;
	m_bLoaded = true;
	return ZFX_OK;
}


