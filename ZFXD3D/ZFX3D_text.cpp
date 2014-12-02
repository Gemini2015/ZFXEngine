#include "ZFXD3D.h"

/**
* Draws text using the previously created font object.
*/
HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, UCHAR r,
	UCHAR g, UCHAR b, char *ch, ...)
{
	RECT rc = { x, y, 0, 0 };
	char cch[1024];
	char *pArgs;

	// put variables into the string
	pArgs = (char*)&ch + sizeof(ch);
	vsprintf(cch, ch, pArgs);

	if (strlen(cch) == 0) return ZFX_OK;
	if (nID >= m_nNumFonts) return ZFX_INVALIDPARAM;

	// 计算字符串宽度
	m_pFont[nID]->DrawText(NULL, cch, -1, &rc, DT_SINGLELINE | DT_CALCRECT, 0);
	// 可以根据rc执行一些额外的操作
	// 真正画字体 
	m_pFont[nID]->DrawText(NULL, cch, -1, &rc, DT_SINGLELINE | DT_CALCRECT, D3DCOLOR_ARGB(255, r, g, b));
	return ZFX_OK;
} // DrawText

HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, DWORD color, char* ch, ...)
{
	RECT rc = { x, y, 0, 0 };
	char cch[1024];
	char *pArgs;

	// put variables into the string
	pArgs = (char*)&ch + sizeof(ch);
	vsprintf(cch, ch, pArgs);

	if (strlen(cch) == 0) return ZFX_OK;
	if (nID >= m_nNumFonts) return ZFX_INVALIDPARAM;

	// calculate actual size of the text
	m_pFont[nID]->DrawText(NULL, cch, -1, &rc, DT_SINGLELINE | DT_CALCRECT, 0);
	// now draw the text 
	m_pFont[nID]->DrawText(NULL, cch, -1, &rc, DT_SINGLELINE | DT_CALCRECT, color);
	return ZFX_OK;
}

HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, DWORD color, char* str)
{
	RECT rc = { x, y, 0, 0 };


	if (strlen(str) == 0) return ZFX_OK;
	if (nID >= m_nNumFonts) return ZFX_INVALIDPARAM;

	// calculate actual size of the text
	m_pFont[nID]->DrawText(NULL, str, -1, &rc, DT_SINGLELINE | DT_CALCRECT, 0);
	// now draw the text 
	m_pFont[nID]->DrawText(NULL, str, -1, &rc, DT_SINGLELINE | DT_CALCRECT, color);
	return ZFX_OK;
}

/*----------------------------------------------------------------*/