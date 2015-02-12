/*
	encoding utf-8
*/
#ifndef _ZFX_FONT_MANAGER_H_
#define _ZFX_FONT_MANAGER_H_


#include "ZFXPrerequisites.h"
#include <string>
#include <map>
#include <vector>

// �������
class Font
{
public:
	enum FontType
	{
		// ȷ����ǰ���������Դ������
		FT_NONE = 0,
		FT_TRUETYPE,
		FT_BITMAP,
	};

	UINT m_uuid;

	// ��������
	std::string m_name;

	// ����Դ�ļ���
	std::string m_sourceName;

	// ��������
	FontType m_type;

	// �ֺ�(������Ϊ��λ)
	float m_size;

	// �����С
	int m_texSize;

	// ���νṹ
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
	
	// ����ӳ�䣬 Skin -> Glyph_Map, CodePoint -> Glyph
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

	// �ڲ������ַ�����
	CodePointRange_Vec m_codePointRangeList;
	// �ݴ��û�������ַ�����
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