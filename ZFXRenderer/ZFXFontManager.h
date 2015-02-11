/*
	encoding utf-8
*/
#ifndef _ZFX_FONT_MANAGER_H_
#define _ZFX_FONT_MANAGER_H_


#include "ZFXPrerequisites.h"
//#include "ZFX.h"
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
	
	SkinGlyph_Map m_skinGlyphMap;
	

public:
	Font(ZFXRenderDevice* pDevice, const char* name, const char* file, float size, UINT nID);
	~Font();

	virtual HRESULT CreateImpl();
	virtual HRESULT DestoryImpl();

	void AddCodePointRange(DWORD from, DWORD to);
	HRESULT LoadFont();
	UINT GetSkinID(DWORD codepoint);
	Glyph GetGlyph(UINT nSkinID, DWORD codepoint);


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

		bool IsIntersect(const tagCodePointRange &range)
		{
			bool intersect = true;
			if (this->to < range.from ||
				this->from >= range.to)
			{
				intersect = false;
			}
			return intersect;
		}

		std::vector<tagCodePointRange> Subtract(const tagCodePointRange &range)
		{
			std::vector<tagCodePointRange> rangelist;
			DWORD from = this->from;
			DWORD to = this->to;
			if (from <= range.from)
			{
				if (to <= range.from)
				{
					rangelist.push_back(*this);
				}
				else if (to <= range.to)
				{
					rangelist.push_back(tagCodePointRange(from, range.from - 1));
				}
				else
				{
					rangelist.push_back(tagCodePointRange(from, range.from - 1));
					rangelist.push_back(tagCodePointRange(range.to + 1, to));
				}
			}
			else if(from <= range.to)
			{
				if (to > range.to)
				{
					rangelist.push_back(tagCodePointRange(range.to + 1, to));
				}
			}
			else
			{
				rangelist.push_back(*this);
			}
			return rangelist;
		}

	}CodePointRange;

	typedef std::vector<CodePointRange> CodePointRange_Vec;
	CodePointRange_Vec m_codePointRangeList;
	
	CodePointRange_Vec m_tempCodePointRangeList;

	int m_nSkinCount;
};

class IFontManager
{
public:
	IFontManager(ZFXRenderDevice* pDevice);
	~IFontManager();

	HRESULT CreateFont(const char* name, const char* file, float size, UINT* nID);
	Font* GetFont(UINT nFontID);

public:
	static UINT s_UUIDCounter;

	typedef std::map<UINT, Font*> Font_Map;
	Font_Map m_fontMap;

private:
	ZFXRenderDevice* m_pDevice;
	
};


#endif