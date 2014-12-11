#include "ZFXOpenGL_VCM.h"


ZFXOpenGLVCacheManager::ZFXOpenGLVCacheManager(ZFXOpenGLSkinManager* pSkinMan, ZFXOpenGL* pOpenGL, UINT nMaxVertx, UINT nMaxIndis)
{

}

ZFXOpenGLVCacheManager::~ZFXOpenGLVCacheManager()
{

}

HRESULT ZFXOpenGLVCacheManager::CreateStaticBuffer(ZFXVERTEXID VertexID, UINT nSkinID, UINT nVerts, UINT nIndis, const void *pVerts, const WORD *pIndis, UINT *pnID)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::CreateIndexBuffer(UINT nIndis, const WORD *pIndis, UINT *pnID)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::Render(ZFXVERTEXID VertexID, UINT nVerts, UINT nIndis, const void *pVerts, const WORD *pIndis, UINT SkinID)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBufferID)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT SBID, UINT IBID, UINT Skin)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBufferID, UINT SkinID, UINT StartIndex, UINT NumVerts, UINT NumTrims)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::RenderNaked(UINT nVerts, const void *pVerts, bool bTextured)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::RenderPoints(ZFXVERTEXID VertexID, UINT nVerts, const void *pVerts, const ZFXCOLOR *pClrl)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::RenderLines(ZFXVERTEXID VertexID, UINT nVerts, const void *pVerts, const ZFXCOLOR *pClrl, bool bStrip)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::RenderLine(const float *fStart, const float *fEnd, const ZFXCOLOR *pClr)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::ForcedFlushAll(void)
{
	throw std::logic_error("The method or operation is not implemented.");
}

HRESULT ZFXOpenGLVCacheManager::ForcedFlush(ZFXVERTEXID)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGLVCacheManager::InvalidateStates(void)
{
	throw std::logic_error("The method or operation is not implemented.");
}

ZFXRENDERSTATE ZFXOpenGLVCacheManager::GetShadeMode(void)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ZFXOpenGLVCacheManager::Log(const char* fmt, ...)
{
	char ch[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf_s(ch, fmt, args);
	va_end(args);

	GetLogger().Print(ch);
}


/************************************************************************/
/* 
	Vertex Cache
*/
/************************************************************************/
ZFXOpenGLVCache::ZFXOpenGLVCache(UINT nVertsMax, UINT nIndisMax, UINT nStride, ZFXOpenGLSkinManager *pSkinMan, ZFXOpenGLVCacheManager *pDad, DWORD dwID, ZFXVERTEXID vid)
{
	m_pSkinMan = pSkinMan;
	m_pVCMan = pDad;
	m_nMaxVertexNum = nVertsMax;
	m_nMaxIndisNum = nIndisMax;
	m_nVertexNum = 0;
	m_nIndisNum = 0;
	m_dwID = dwID;
	m_vid = vid;
	m_nStride = nStride;

	m_VertexBuffer = 0;
	m_IndexBuffer = 0;

	m_Skin.bAlpha = false;
	m_Skin.nMaterial = UINT_MAX;
	memset(&m_Skin.nTexture[0], ZFXOpenGLSkinManager::MAX_ID, 8 * sizeof(UINT));
	m_SkinID = ZFXOpenGLSkinManager::MAX_ID;

	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	GLsizeiptr size = nVertsMax * nStride;
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	
	SetFVF(vid);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	size = nIndisMax * sizeof(UINT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

ZFXOpenGLVCache::~ZFXOpenGLVCache()
{
	if (glIsBuffer(m_VertexBuffer))
		glDeleteBuffers(1, &m_VertexBuffer);

	if (glIsBuffer(m_IndexBuffer))
		glDeleteBuffers(1, &m_IndexBuffer);
}

HRESULT ZFXOpenGLVCache::SetFVF(ZFXVERTEXID vid)
{
	HRESULT hr = ZFX_OK;
	switch (vid)
	{
	case VID_PS:
		glVertexPointer(3, GL_FLOAT, 3 * sizeof(PVERTEX), 0);
		break;
	case VID_UU:
		glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 3));
		glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_UL:
		glVertexPointer(3, GL_FLOAT, sizeof(LVERTEX), 0);
		glColorPointer(4, GL_UNSIGNED_INT, sizeof(LVERTEX), (void*)(sizeof(float) * 3));
		glTexCoordPointer(2, GL_FLOAT, sizeof(LVERTEX), (void*)(sizeof(float) * 3 + sizeof(DWORD)));
		break;
	case VID_CA:
		glVertexPointer(3, GL_FLOAT, sizeof(CVERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 3));
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_3T:
		glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 3));
		glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 6));
		// 
		// glEnableClientState(
		break;
	case VID_TV:
		break;
	default:
		hr = E_INVALIDARG;
		break;
	}
	return hr;
}

HRESULT ZFXOpenGLVCache::Flush(bool bUseShaders)
{
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCache::Add(UINT nVerts, UINT nIndis, const void *pVerts, const WORD *pIndis, bool bUseShaders)
{
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCache::SetSkin(UINT SkinID, bool bUseShader)
{
	// if a new texture is coming, flush all content of
	// the cache because that is using other textures
	if (!IsUseSkin(SkinID)) 
	{
		ZFXSKIN *pSkin = &m_pSkinMan->GetSkin(SkinID);

		if (!IsEmpty()) Flush(bUseShader);

		memcpy(&m_Skin, pSkin, sizeof(ZFXSKIN));
		m_SkinID = SkinID;

		// skin changing so active cache is invalid any case
		m_pVCMan->InvalidateStates();
	}
	return ZFX_OK;
}

void ZFXOpenGLVCache::Log(const char* fmt, ...)
{
	char ch[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf_s(ch, fmt, args);
	va_end(args);

	GetLogger().Print(ch);
}

