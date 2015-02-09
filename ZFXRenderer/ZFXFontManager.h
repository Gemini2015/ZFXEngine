/*
	encoding utf-8
*/
#ifndef _ZFX_FONT_MANAGER_H_
#define _ZFX_FONT_MANAGER_H_

#include "ZFXPrerequisites.h"
#include<string>
#include<map>

// �������
class Font
{
public:
	enum FontType
	{
		// ȷ����ǰ���������Դ������
		FT_TRUETYPE = 0,
		FT_BITMAP = 1,
	};

	// ��������
	std::string m_name;

	// ����Դ�ļ���
	std::string m_sourceName;

	// ��������
	FontType m_type;

	// �ֺ�(������Ϊ��λ)
	float m_size;

	// ���νṹ
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

	// �ַ�ӳ���
	Glyph_Map m_glyphMap;

	// �����Ӧ��SkinID
	UINT m_nSkinID;
};

class IFontManager
{
public:
	typedef std::map<UINT, Font*> Font_Map;
	Font_Map m_fontMap;
};


#endif