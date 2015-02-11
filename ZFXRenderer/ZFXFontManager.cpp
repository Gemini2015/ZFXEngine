/*
	encoding: utf-8
*/
#include "ZFXInclude.h"
#include "ZFXFontManager.h"
#include "ft2build.h"

#ifdef _DEBUG

#include "freetype.h"
#include "fterrors.h"

#else

#include FT_FREETYPE_H
#include FT_ERRORS_H

#endif



Font::Font(ZFXRenderDevice* pDevice, const char* name, const char* file, float size, UINT nID)
{
	m_name.assign(name);
	m_sourceName.assign(file);
	m_size = size;
	m_uuid = nID;
	m_type = FT_NONE;
	m_pDevice = pDevice;
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

void Font::AddCodePointRange(DWORD from, DWORD to)
{
	if (from >= to)
		return;
	CodePointRange range(from, to);
	m_tempCodePointRangeList.push_back(range);
}

UINT Font::GetSkinID(DWORD codepoint)
{
	UINT skinID = -1;
	CodePointRange_Vec::iterator it = m_codePointRangeList.begin();
	while (it != m_codePointRangeList.end())
	{
		if (codepoint >= it->from && codepoint <= it->to)
		{
			skinID = it->nSkinID;
			break;
		}
		it++;
	}
	return skinID;
}

Font::Glyph Font::GetGlyph(DWORD codepoint)
{
	Glyph_Map::iterator it = m_glyphMap.find(codepoint);
	if (it != m_glyphMap.end())
	{
		return it->second;
	}
	else
	{
		Glyph glyph;
		return glyph;
	}
}

HRESULT Font::LoadFont()
{
	if (m_pDevice == NULL)
		return ZFX_FAIL;
	ZFXSkinManager* pSkinManager = m_pDevice->GetSkinManager();
	if (pSkinManager == NULL)
		return ZFX_FAIL;

	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		GetLogger().Print("error in FT_Init_FreeType");
		return ZFX_FAIL;
	}

	FT_Face face;
	error = FT_New_Face(library, m_sourceName.c_str(), 0, &face);
	if (error)
	{
		GetLogger().Print("error in FT_New_Face");
		return ZFX_FAIL;
	}

	error = FT_Set_Pixel_Sizes(face, m_size, 0);
	if (error)
	{
		GetLogger().Print("error in FT_Set_Pixel_Sizes");
		return ZFX_FAIL;
	}

	if (m_tempCodePointRangeList.empty())
	{
		CodePointRange range(33, 255);
		m_tempCodePointRangeList.push_back(range);
	}

	// 判断字体类型
	m_type = face->face_flags & FT_FACE_FLAG_SCALABLE ? FT_TRUETYPE : FT_BITMAP;

	// 获取Cell大小
	int max_width = 0, max_height = 0;
	CodePointRange_Vec::iterator it = m_tempCodePointRangeList.begin();
	while (it != m_tempCodePointRangeList.begin())
	{
		//遍历每个区间
		DWORD from = it->from;
		DWORD to = it->to;
		for (DWORD i = from; i <= to; i++)
		{
			// 遍历每个字符
			error = FT_Load_Char(face, i, FT_LOAD_RENDER);
			if (error)
			{
				continue;
			}
			
			if (face->glyph->bitmap.rows > max_height)
				max_height = face->glyph->bitmap.rows;
			if (face->glyph->advance.x > max_width)
				max_width = face->glyph->advance.x;
		}
		it++;
	}

	// 创建一张 2048 * 2048 的纹理
	ZFXIMAGE img;
	img.width = 2048;
	img.height = 2048;
	int image_size = img.width * img.height;
	img.data = new unsigned char[image_size];
	memset(img.data, 0, image_size);

	UINT nSkinID = 0;
	ZFXCOLOR color(1, 1, 1, 1);
	ZFXCOLOR emissive(0, 0, 0, 0);
	pSkinManager->AddSkin(&color, &color, &emissive, &emissive, 50, &nSkinID);

	int penx = 0;
	int peny = 0;
	int rangefrom = 0;
	int rangeto = 0;

	it = m_tempCodePointRangeList.begin();
	while (it != m_tempCodePointRangeList.begin())
	{
		//遍历每个区间
		DWORD from = it->from;
		DWORD to = it->to;
		rangefrom = from;
		rangeto = to;
		for (DWORD codepoint = from; codepoint <= to; codepoint++, rangeto++)
		{
			// 遍历每个字符
			error = FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
			if (error)
			{
				continue;
			}
			
			FT_GlyphSlot slot = face->glyph;

			int right = penx + max_width;//slot->advance.x;
			int bottom = peny + 2 * max_height;

			if (right > img.width &&
				bottom > img.height)
			{
				// 当前纹理已填充满
				char buf[255] = { 0 };
				sprintf_s(buf, "%s-%d", m_name.c_str(), m_nSkinCount);
				pSkinManager->AddTextureFromMemory(nSkinID, buf, &img, false, 1.0f, NULL, 0);

				rangeto--;
				CodePointRange range(rangefrom, rangeto);
				range.nSkinID = nSkinID;

				rangefrom = rangeto + 1;
				m_nSkinCount++;

				// 创建一张新的纹理
				pSkinManager->AddSkin(&color, &color, &emissive, &emissive, 50, &nSkinID);
				
				penx = peny = 0;
				right = penx + slot->advance.x;
				bottom = peny + slot->bitmap.rows;
			}

			if (right > img.width)
			{
				// 换行
				penx = 0;
				peny += max_height;
				right = penx + slot->advance.x;
				bottom = peny + slot->bitmap.rows;
			}

			unsigned char* pBuf = (unsigned char*)img.data;
			for (int row = peny; row < peny + slot->bitmap.rows; row++)
			{
				for (int col = penx; col < penx + slot->advance.x; col++)
				{
					int x = col - penx;
					int y = row - peny;
					pBuf[row * img.width + col] = slot->bitmap.buffer[y * slot->bitmap.pitch + x];
				}
			}

			// 此处涉及到 y 轴方向的问题

			Glyph glyph(codepoint,
				(float)penx / img.width,
				(float)peny / img.height,
				(float)right / img.width,
				(float)bottom / img.height);

			// 若出现重合字符，此处会出问题
			m_glyphMap[codepoint] = glyph;

		}

		// 区间结束
		CodePointRange range(rangefrom, rangeto);
		range.nSkinID = nSkinID;
		m_codePointRangeList.push_back(range);

		it++;
	}

	char buf[255] = { 0 };
	sprintf_s(buf, "%s-%d", m_name.c_str(), m_nSkinCount);
	pSkinManager->AddTextureFromMemory(nSkinID, buf, &img, false, 1.0f, NULL, 0);
	m_nSkinCount++;

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return ZFX_OK;
}


UINT IFontManager::s_UUIDCounter = 0;
IFontManager::IFontManager(ZFXRenderDevice *pDevice)
{
	m_pDevice = pDevice;
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
	Font* font = new Font(m_pDevice, name, file, size, id);

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
