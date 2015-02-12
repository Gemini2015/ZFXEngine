/*
	encoding utf-8
*/
#ifndef _ZFX_FONT_MANAGER_H_
#define _ZFX_FONT_MANAGER_H_


#include "ZFXPrerequisites.h"
#include <string>
#include <map>
#include <vector>

// 字体抽象
class Font
{
public:
	enum FontType
	{
		// 确定当前载入的字体源的类型
		FT_NONE = 0,
		FT_TRUETYPE,
		FT_BITMAP,
	};

	UINT m_uuid;

	// 字体名称
	std::string m_name;

	// 字体源文件名
	std::string m_sourceName;

	// 字体类型
	FontType m_type;

	// 字号(以像素为单位)
	float m_size;

	// 纹理大小
	int m_texSize;

	// 字形结构
	typedef struct tagGlyph
	{
		DWORD codepoint;
		float left;
		float top;
		float right;
		float bottom;

		tagGlyph()
		{
			codepoint = 0;
			left = top = right = bottom = 0;
		}

		tagGlyph(DWORD codepoint, float left, float top, float right, float bottom)
		{
			this->codepoint = codepoint;
			this->left = left;
			this->top = top;
			this->right = right;
			this->bottom = bottom;
		}

	}Glyph;

	typedef std::map<DWORD, Glyph> Glyph_Map;
	typedef std::map<UINT, Glyph_Map> SkinGlyph_Map;
	
	// 二重映射， Skin -> Glyph_Map, CodePoint -> Glyph
	SkinGlyph_Map m_skinGlyphMap;
	

public:
	Font(ZFXRenderDevice* pDevice, const char* name, const char* file, int texSize, float fontSize, UINT nID);
	~Font();

	virtual HRESULT CreateImpl();
	virtual HRESULT DestoryImpl();

	void AddCodePointRange(DWORD from, DWORD to);
	HRESULT LoadFont();
	UINT GetSkinID(DWORD codepoint);
	Glyph GetGlyph(UINT nSkinID, DWORD codepoint);
	float GetFontSize()
	{
		return m_size;
	}

private:
	ZFXRenderDevice* m_pDevice;

	typedef struct tagCodePointRange
	{
		DWORD from;
		DWORD to;
		UINT nSkinID;

		tagCodePointRange()
		{
			from = to = 0;
			nSkinID = -1;
		}

		tagCodePointRange(DWORD from, DWORD to)
		{
			this->from = from;
			this->to = to;
		}
	}CodePointRange;

	typedef std::vector<CodePointRange> CodePointRange_Vec;

	// 内部保存字符区间
	CodePointRange_Vec m_codePointRangeList;
	// 暂存用户输入的字符区间
	CodePointRange_Vec m_tempCodePointRangeList;

	virtual UINT CreateSkin();
	virtual HRESULT UpdateSkinTexture(UINT nSkinID, ZFXIMAGE *img);
};

class IFontManager
{
public:
	IFontManager(ZFXRenderDevice* pDevice);
	~IFontManager();

	HRESULT CreateFont(const char* name, const char* file, float size, UINT* nID, int texSize = 2048);
	Font* GetFont(UINT nFontID);

public:
	static UINT s_UUIDCounter;

	typedef std::map<UINT, Font*> Font_Map;
	Font_Map m_fontMap;

private:
	ZFXRenderDevice* m_pDevice;
	
};


#endif