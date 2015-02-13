#include "ZFXD3D.h"


/**
* Creates a D3DXFont object from a GDI font the caller submitted
* and returns its ID to the caller for later use.
*/
HRESULT ZFXD3D::CreateFont(const char *chType, int nWeight, bool bItalic,
	bool bUnderline, bool bStrike, DWORD dwSize,
	UINT *pID) {
	HRESULT hr;
	LPD3DXFONT font;
	HDC     hDC;
	int     nHeight;

	if (!pID) return ZFX_INVALIDPARAM;

	hDC = GetDC(NULL);
	nHeight = -MulDiv(dwSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	ReleaseDC(NULL, hDC);

	// 创建字体
	hr = D3DXCreateFont(m_pDevice, nHeight, 0, FW_BOLD, 1, bItalic,
		DEFAULT_CHARSET, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, chType, &font);

	if (FAILED(hr)) return ZFX_FAIL;

	// 加入字体列表
	m_pFont = (LPD3DXFONT*)realloc(m_pFont, sizeof(LPD3DXFONT)*(m_nNumFonts + 1));

	m_pFont[m_nNumFonts] = font;
	(*pID) = m_nNumFonts;
	m_nNumFonts++;
	return ZFX_OK;

} // CreateFont
/*----------------------------------------------------------------*/

/**
* Draws text using the previously created font object.
*/
HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, UCHAR r,
	UCHAR g, UCHAR b, const char *ch, ...)
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

HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, DWORD color, const char* ch, ...)
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

HRESULT ZFXD3D::DrawText(UINT nID, int x, int y, DWORD color, const char* str)
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