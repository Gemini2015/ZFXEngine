#ifndef _ZFXOPENGL_VCM_H_
#define _ZFXOPENGL_VCM_H_

#include "ZFXRenderDevice.h"
#include "ZFXOpenGL.h"
#include "ZFXOpenGL_Skin.h"

class ZFXOpenGLVCacheManager : public ZFXVertexCacheManager
{
public:
	ZFXOpenGLVCacheManager(ZFXOpenGLSkinManager* pSkinMan,
		ZFXOpenGL* pOpenGL,
		UINT nMaxVertx,
		UINT nMaxIndis);
	~ZFXOpenGLVCacheManager();

	virtual HRESULT CreateStaticBuffer(ZFXVERTEXID VertexID,
		UINT nSkinID, 
		UINT nVerts, 
		UINT nIndis, 
		const void *pVerts, 
		const WORD *pIndis, 
		UINT *pnID) override;

	virtual HRESULT CreateIndexBuffer(UINT nIndis, 
		const WORD *pIndis, 
		UINT *pnID) override;

	virtual HRESULT Render(ZFXVERTEXID VertexID, 
		UINT nVerts, 
		UINT nIndis, 
		const void *pVerts, 
		const WORD *pIndis, 
		UINT SkinID) override;

	virtual HRESULT Render(UINT nSBufferID) override;

	virtual HRESULT Render(UINT SBID, UINT IBID, UINT Skin) override;

	virtual HRESULT Render(UINT nSBufferID, 
		UINT SkinID, 
		UINT StartIndex, 
		UINT NumVerts, 
		UINT NumTrims) override;

	virtual HRESULT RenderNaked(UINT nVerts, 
		const void *pVerts, 
		bool bTextured) override;

	virtual HRESULT RenderPoints(ZFXVERTEXID VertexID, 
		UINT nVerts, 
		const void *pVerts, 
		const ZFXCOLOR *pClrl) override;

	virtual HRESULT RenderLines(ZFXVERTEXID VertexID, 
		UINT nVerts, 
		const void *pVerts, 
		const ZFXCOLOR *pClrl, 
		bool bStrip) override;

	virtual HRESULT RenderLine(const float *fStart, 
		const float *fEnd, 
		const ZFXCOLOR *pClr) override;

	virtual HRESULT ForcedFlushAll(void) override;

	virtual HRESULT ForcedFlush(ZFXVERTEXID) override;

	virtual void InvalidateStates(void) override;

	virtual ZFXRENDERSTATE GetShadeMode(void) override;

private:
	ZFXOpenGLSkinManager* m_pSkinMan;
	ZFXOpenGL* m_pOpenGL;


	void Log(const char* fmt, ...);
};


class ZFXOpenGLVCache
{
public:
	ZFXOpenGLVCache(UINT nVertsMax, 
		UINT nIndisMax,
		UINT nStride, 
		ZFXOpenGLSkinManager *pSkinMan,
		ZFXOpenGLVCacheManager *pDad,
		DWORD dwID, 
		ZFXVERTEXID vid);
	~ZFXOpenGLVCache();

	HRESULT SetFVF(ZFXVERTEXID vid);

	HRESULT Flush(bool bUseShaders);

	HRESULT Add(UINT nVerts, 
		UINT nIndis,
		const void *pVerts,
		const WORD *pIndis,
		bool  bUseShaders);

	HRESULT SetSkin(UINT SkinID, bool bUseShader);
	bool IsUseSkin(UINT SkinID) 
	{
		return (m_SkinID == SkinID);
	}
	bool IsEmpty()
	{
		if (m_nVertexNum > 0)
			return false;
		return true;
	}
	int GetVertexNum()
	{
		return m_nVertexNum;
	}

private:
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
	ZFXOpenGLSkinManager *m_pSkinMan;
	ZFXOpenGLVCacheManager *m_pVCMan;
	ZFXSKIN m_Skin;
	UINT m_SkinID;
	DWORD m_dwID;
	ZFXVERTEXID m_vid;

	UINT m_nMaxVertexNum;
	UINT m_nMaxIndisNum;
	UINT m_nVertexNum;
	UINT m_nIndisNum;
	UINT m_nStride;

	void Log(const char* fmt, ...);
	
};

#endif