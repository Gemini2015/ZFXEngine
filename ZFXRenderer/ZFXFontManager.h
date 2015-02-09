/*
	encoding utf-8
*/
#ifndef _ZFX_FONT_MANAGER_H_
#define _ZFX_FONT_MANAGER_H_

#include "ZFXPrerequisites.h"
#include<string>
#include<map>

// 字体抽象
class Font
{
public:
	enum FontType
	{
		// 确定当前载入的字体源的类型
		FT_TRUETYPE = 0,
		FT_BITMAP = 1,
	};

	// 字体名称
	std::string m_name;

	// 字体源文件名
	std::string m_sourceName;

	// 字体类型
	FontType m_type;

	// 字号(以像素为单位)
	float m_size;

	// 字形结构
	typedef struct tagGlyph
	{
		DWORD charcode;
		float left;
		float top;
		float right;
		float bottom;

		tagGlyph()
		{
			charcode = 0;
			left = top = right = bottom = 0;
		}

	}Glyph;

	
	typedef std::map<DWORD, Glyph> Glyph_Map;

	// 字符映射表
	Glyph_Map m_glyphMap;

	// 字体对应的SkinID
	UINT m_nSkinID;
};

class IFontManager
{
public:
	typedef std::map<UINT, Font*> Font_Map;
	Font_Map m_fontMap;
};


#endif