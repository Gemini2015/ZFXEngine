/*
	encoding: utf-8
*/
#include "ZFXFontManager.h"

Font::Font(const char* name, const char* file, float size, UINT nID)
{
	m_name.assign(name);
	m_sourceName.assign(file);
	m_size = size;
	m_uuid = nID;
	m_type = FT_NONE;
	m_nSkinID = 0;
}

Font::~Font()
{
}

HRESULT Font::CreateImpl()
{
	return ZFX_OK;
}

HRESULT Font::DestoryImpl()
{
	return ZFX_OK;
}

HRESULT Font::CreateTextureFromFont(ZFXIMAGE *img)
{
	return ZFX_OK;
}


UINT IFontManager::s_UUIDCounter = 0;
IFontManager::IFontManager()
{

}

IFontManager::~IFontManager()
{
	Font_Map::iterator it = m_fontMap.begin();
	while (it != m_fontMap.end())
	{
		if (it->second)
		{
			it->second->DestoryImpl();
			delete it->second;
		}
		it++;
	}
}

HRESULT IFontManager::CreateFont(const char* name, const char* file, float size, UINT* nID)
{
	if (name == NULL || strlen(name) == 0 ||
		file == NULL || strlen(file) == 0 ||
		size == 0)
		return ZFX_INVALIDPARAM;

	UINT id = ++s_UUIDCounter;
	Font* font = new Font(name, file, size, id);

	HRESULT hr = font->CreateImpl();
	if(!FAILED(hr))
	{
		m_fontMap[id] = font;
		if (nID)
		{
			*nID = id;
		}
	}
	else
	{
		delete font;
	}
	return hr;
}

Font* IFontManager::GetFont(UINT nFontID)
{
	Font_Map::iterator it = m_fontMap.find(nFontID);
	if (it != m_fontMap.end())
	{
		return it->second;
	}
	else return NULL;
}
