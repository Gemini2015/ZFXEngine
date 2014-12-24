#include "ZFXOpenGL_VCM.h"
#include "ZFXOpenGL.h"

ZFXOpenGLVCacheManager::ZFXOpenGLVCacheManager(ZFXOpenGLSkinManager* pSkinMan,
	ZFXOpenGL* pOpenGL, 
	UINT nMaxVertx, 
	UINT nMaxIndis)
{
	m_pOpenGL = pOpenGL;
	m_pSkinMan = pSkinMan;

	m_pStaticBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_nStaticBufferNum = 0;
	m_nIndexBufferNum = 0;

	m_dwActiveVCache = MAX_ID;
	m_dwActiveStaticBuffer = MAX_ID;
	m_dwActiveIndexBuffer = MAX_ID;

	DWORD dwID = 1;
	for (int i = 0; i < ZFXOpenGLVCache_NUM; i++)
	{
		m_pCachePS[i] = new ZFXOpenGLVCache(nMaxVertx, 
			nMaxIndis, sizeof(PVERTEX),	pSkinMan, 
			this, dwID++, VID_PS);

		m_pCacheUU[i] = new ZFXOpenGLVCache(nMaxVertx,
			nMaxIndis, sizeof(PVERTEX), pSkinMan,
			this, dwID++, VID_UU);

		m_pCacheUL[i] = new ZFXOpenGLVCache(nMaxVertx,
			nMaxIndis, sizeof(PVERTEX), pSkinMan,
			this, dwID++, VID_UL);

		m_pCacheCA[i] = new ZFXOpenGLVCache(nMaxVertx,
			nMaxIndis, sizeof(PVERTEX), pSkinMan,
			this, dwID++, VID_CA);

		m_pCache3T[i] = new ZFXOpenGLVCache(nMaxVertx,
			nMaxIndis, sizeof(PVERTEX), pSkinMan,
			this, dwID++, VID_3T);

		m_pCacheTV[i] = new ZFXOpenGLVCache(nMaxVertx,
			nMaxIndis, sizeof(PVERTEX), pSkinMan,
			this, dwID++, VID_TV);
	}

	Log("Create OpenGL Vertex Cache Manager");
}

ZFXOpenGLVCacheManager::~ZFXOpenGLVCacheManager()
{
	UINT n = 0;
	if (m_pStaticBuffer)
	{
		for (int i = 0; i < m_nStaticBufferNum; i++)
		{
			if (glIsBuffer(m_pStaticBuffer[i].VertexBuffer))
			{
				glDeleteBuffers(1, &m_pStaticBuffer[i].VertexBuffer);
				m_pStaticBuffer[i].VertexBuffer = 0;
			}
			if (glIsBuffer(m_pStaticBuffer[i].IndisBuffer))
			{
				glDeleteBuffers(1, &m_pStaticBuffer[i].IndisBuffer);
				m_pStaticBuffer[i].IndisBuffer = 0;
			}
		}
		delete[] m_pStaticBuffer;
		m_pStaticBuffer = NULL;
	}
	if (m_pIndexBuffer)
	{
		for (int i = 0; i < m_nIndexBufferNum; i++)
		{
			if (glIsBuffer(m_pIndexBuffer[i].IndisBuffer))
			{
				glDeleteBuffers(1, &m_pIndexBuffer[i].IndisBuffer);
				m_pIndexBuffer[i].IndisBuffer = 0;
			}
		}
		delete[] m_pIndexBuffer;
		m_pIndexBuffer = NULL;
	}

	for (int i = 0; i < ZFXOpenGLVCache_NUM; i++)
	{
		if (m_pCachePS[i])
		{
			delete m_pCachePS[i];
			m_pCachePS[i] = NULL;
		}
		if (m_pCacheUU[i])
		{
			delete m_pCacheUU[i];
			m_pCacheUU[i] = NULL;
		}
		if (m_pCacheCA[i])
		{
			delete m_pCacheCA[i];
			m_pCacheCA[i] = NULL;
		}
		if (m_pCache3T[i])
		{
			delete m_pCache3T[i];
			m_pCache3T[i] = NULL;
		}
		if (m_pCacheTV[i])
		{
			delete m_pCacheTV[i];
			m_pCacheTV[i] = NULL;
		}
		if (m_pCacheUL[i])
		{
			delete m_pCacheUL[i];
			m_pCacheUL[i] = NULL;
		}
	}
}

HRESULT ZFXOpenGLVCacheManager::CreateStaticBuffer(ZFXVERTEXID VertexID, 
	UINT nSkinID, 
	UINT nVerts, 
	UINT nIndis, 
	const void *pVerts, 
	const WORD *pIndis, 
	UINT *pnID)
{
	if (m_nStaticBufferNum >= (MAX_ID - 1))
		return ZFX_OUTOFMEMORY;
	
	if ((m_nStaticBufferNum % 50) == 0)
	{
		int n = (m_nStaticBufferNum + 50) * sizeof(ZFXSTATICBUFFER);
		m_pStaticBuffer = (ZFXSTATICBUFFER *)realloc(m_pStaticBuffer, n);
		if (!m_pStaticBuffer) return ZFX_OUTOFMEMORY;
	}

	m_pStaticBuffer[m_nStaticBufferNum].nVertexNum = nVerts;
	m_pStaticBuffer[m_nStaticBufferNum].nIndisNum = nIndis;
	m_pStaticBuffer[m_nStaticBufferNum].nSkinID = nSkinID;
	m_pStaticBuffer[m_nStaticBufferNum].VertexBuffer = 0;
	m_pStaticBuffer[m_nStaticBufferNum].IndisBuffer = 0;

	switch (VertexID)
	{
	case VID_PS:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(PVERTEX);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_PS;
		break;
	case VID_UU:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(VERTEX);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_UU;
		break;
	case VID_UL:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(LVERTEX);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_UL;
		break;
	case VID_CA:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(CVERTEX);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_CA;
		break;
	case VID_3T:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(VERTEX3T);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_3T;
		break;
	case VID_TV:
		m_pStaticBuffer[m_nStaticBufferNum].nStride = sizeof(TVERTEX);
		m_pStaticBuffer[m_nStaticBufferNum].nVertexType = VID_TV;
		break;
	default:
		return ZFX_INVALIDID;	// 返回是否会发生内存溢出？
	}

	if (nIndis > 0)
	{
		m_pStaticBuffer[m_nStaticBufferNum].bIndis = true;
		m_pStaticBuffer[m_nStaticBufferNum].nTriangleNum = (int)(nIndis / 3.0f);

		GLuint indexbuffer = 0;
		glGenBuffers(1, &indexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
		CHECK_ERROR;
		GLsizeiptr size = nIndis * sizeof(WORD);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pIndis, GL_STATIC_DRAW);
		CHECK_ERROR;
		m_pStaticBuffer[m_nStaticBufferNum].IndisBuffer = indexbuffer;
	}
	else
	{
		m_pStaticBuffer[m_nStaticBufferNum].bIndis = false;
		m_pStaticBuffer[m_nStaticBufferNum].nTriangleNum = (int)(nVerts / 3.0f);
		m_pStaticBuffer[m_nStaticBufferNum].IndisBuffer = 0;
	}

	GLuint vertexbuffer = 0;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	GLsizeiptr size = nVerts * m_pStaticBuffer[m_nStaticBufferNum].nStride;
	glBufferData(GL_ARRAY_BUFFER, size, pVerts, GL_STATIC_DRAW);
	CHECK_ERROR;
	m_pStaticBuffer[m_nStaticBufferNum].VertexBuffer = vertexbuffer;

	(*pnID) = m_nStaticBufferNum;
	m_nStaticBufferNum++;
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::CreateIndexBuffer(UINT nIndis, const WORD *pIndis, UINT *pnID)
{
	if (m_nIndexBufferNum >= (MAX_ID - 1)) return ZFX_OUTOFMEMORY;

	if ((m_nIndexBufferNum % 50) == 0)
	{
		int n = (m_nIndexBufferNum + 50) * sizeof(ZFXINDEXBUFFER);
		m_pIndexBuffer = (ZFXINDEXBUFFER *)realloc(m_pIndexBuffer, n);
		if (!m_pIndexBuffer) return ZFX_OUTOFMEMORY;
	}

	m_pIndexBuffer[m_nIndexBufferNum].nIndisNum = nIndis;
	m_pIndexBuffer[m_nIndexBufferNum].nTriangleNum = (int)(nIndis / 3.0f);
	m_pIndexBuffer[m_nIndexBufferNum].IndisBuffer = 0;

	GLuint indexbuffer = 0;
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndis * sizeof(WORD), pIndis, GL_STATIC_DRAW);

	m_pIndexBuffer[m_nIndexBufferNum].IndisBuffer = indexbuffer;
	(*pnID) = m_nIndexBufferNum;
	m_nIndexBufferNum++;
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::Render(ZFXVERTEXID VertexID, 
	UINT nVerts, UINT nIndis, const void *pVerts, const WORD *pIndis, 
	UINT SkinID)
{
	ZFXOpenGLVCache** pCache = NULL;
	ZFXOpenGLVCache *pCacheEmpty = NULL, *pCacheFullest = NULL;
	int nEmptyVCache = -1;
	int nFullestVCache = 0;

	bool bUseShader = m_pOpenGL->IsUseShaders();

	switch (VertexID)
	{
	case VID_PS: { pCache = m_pCachePS; } break;
	case VID_UU: { pCache = m_pCacheUU; } break;
	case VID_UL: { pCache = m_pCacheUL; } break;
	case VID_CA: { pCache = m_pCacheCA; } break;
	case VID_3T: { pCache = m_pCache3T; } break;
	case VID_TV: { pCache = m_pCacheTV; } break;
		// unknown vertex type
	default: return ZFX_INVALIDID;
	} // switch

	pCacheFullest = pCache[0];
	m_dwActiveStaticBuffer = MAX_ID;
	
	for (int i = 0; i < ZFXOpenGLVCache_NUM; i++)
	{
		if (pCache[i]->IsUseSkin(SkinID))
		{
			return pCache[i]->Add(nVerts, nIndis, pVerts, pIndis, bUseShader);
		}

		if (pCache[i]->IsEmpty())
			pCacheEmpty = pCache[i];

		if (pCache[i]->GetVertexNum() > pCacheFullest->GetVertexNum())
			pCacheFullest = pCache[i];
	}

	if (pCacheEmpty)
	{
		pCacheEmpty->SetSkin(SkinID, bUseShader);
		return pCacheEmpty->Add(nVerts, nIndis, pVerts, pIndis, bUseShader);
	}

	pCacheFullest->Flush(bUseShader);
	pCacheFullest->SetSkin(SkinID, bUseShader);
	return pCacheFullest->Add(nVerts, nIndis, pVerts, pIndis, bUseShader);
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBID)
{
	HRESULT hr = ZFX_OK;

	if (m_pStaticBuffer == NULL)
	{
		Log("Error Static Buffer NULL pointer");
		return ZFX_FAIL;
	}
	if (nSBID >= MAX_ID)
	{
		Log("Error Invalid Static Buffer ID");
		return ZFX_INVALIDPARAM;
	}

	m_dwActiveVCache = MAX_ID;

	if (m_dwActiveStaticBuffer != nSBID)
	{
		if (m_pStaticBuffer[nSBID].bIndis)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pStaticBuffer[nSBID].IndisBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_pStaticBuffer[nSBID].VertexBuffer);
			m_dwActiveStaticBuffer = nSBID;
		}
	}
	else if (m_dwActiveIndexBuffer != MAX_ID)
	{
		if (m_pStaticBuffer[nSBID].bIndis)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pStaticBuffer[nSBID].IndisBuffer);
			CHECK_ERROR;
		}
		m_dwActiveIndexBuffer = MAX_ID;
	}

	if (m_pOpenGL->GetActiveSkinID() != m_pStaticBuffer[nSBID].nSkinID)
	{
		hr = m_pOpenGL->ActiveSkin(m_pStaticBuffer[nSBID].nSkinID);
		if (FAILED(hr))
			return hr;
	} // set device skin

	if (!m_pOpenGL->IsUseShaders())
	{
		// set FVF
		ZFXOpenGLVCache::SetFVF(m_pStaticBuffer[nSBID].nVertexType);
	}

	if (m_pOpenGL->IsUseAdditiveBlending())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	if (!m_pOpenGL->IsUseColorBuffer())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE);
	}

	CHECK_ERROR;

	// Enable vertex array
	hr = ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, true);

	int nVertexNum = m_pStaticBuffer[nSBID].nVertexNum;
	int nIndisNum = m_pStaticBuffer[nSBID].nIndisNum;
	ZFXRENDERSTATE mode = m_pOpenGL->GetShadeMode();
	
	hr = DrawPrimitive(mode, nVertexNum, nIndisNum, m_pStaticBuffer[nSBID].bIndis);

	hr = ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, false);
	
	CHECK_ERROR;
	return hr;
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBID, UINT nIBID, UINT nSkin)
{
	HRESULT hr = ZFX_OK;

	m_dwActiveVCache = MAX_ID;
	
	if ((nSBID >= m_nStaticBufferNum) || (nIBID >= m_nIndexBufferNum))
	{
		Log("Error Invalid Buffer ID");
		return ZFX_INVALIDPARAM;
	}

	if (m_dwActiveStaticBuffer != nSBID)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_pStaticBuffer[nSBID].VertexBuffer);
		m_dwActiveStaticBuffer = nSBID;
	}
	if (m_dwActiveIndexBuffer != nIBID)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer[nIBID].IndisBuffer);
		m_dwActiveIndexBuffer = nIBID;
	}

	if (m_pOpenGL->GetActiveSkinID() != nSkin)
	{
		m_pOpenGL->ActiveSkin(nSkin);
	}// set skin

	if (!m_pOpenGL->IsUseShaders())
	{
		// Set FVF
		ZFXOpenGLVCache::SetFVF(m_pStaticBuffer[nSBID].nVertexType);
	}

	if (m_pOpenGL->IsUseAdditiveBlending())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	if (!m_pOpenGL->IsUseColorBuffer())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE);
	}

	int nVertexNum = m_pStaticBuffer[nSBID].nVertexNum;
	int nIndisNum = m_pStaticBuffer[nSBID].nIndisNum;
	ZFXRENDERSTATE mode = m_pOpenGL->GetShadeMode();
	//// index primitive
	//// 没有实现StartIndex
	//switch (rs)
	//{
	//case RS_SHADE_POINTS:     // render just vertices
	//	glDrawArrays(GL_POINTS, 0, nVertexNum);
	//	break;
	//case RS_SHADE_LINES:      // render two verts as one line
	//	glDrawElements(GL_LINES, nIndisNum / 2, GL_UNSIGNED_SHORT, 0);
	//	break;
	//case RS_SHADE_HULLWIRE:   // render poly hull as polyline
	//	glDrawElements(GL_LINE_STRIP, nIndisNum, GL_UNSIGNED_SHORT, 0);
	//	break;
	//case RS_SHADE_TRIWIRE:    // render triangulated wire
	//case RS_SHADE_SOLID:	   // render solid polygons
	//default:
	//	glDrawElements(GL_TRIANGLES, nIndisNum / 3, GL_UNSIGNED_SHORT, 0);
	//	break;
	//}

	ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, true);

	hr = DrawPrimitive(mode, nVertexNum, nIndisNum, true);

	ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, false);

	return hr;
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBID, UINT SkinID, UINT StartIndex, UINT NumVerts, UINT NumTrims)
{
	HRESULT hr = ZFX_OK;

	m_dwActiveVCache = MAX_ID;
	if (nSBID >= m_nStaticBufferNum)
	{
		Log("Error Invalid Static Buffer ID");
		return ZFX_INVALIDPARAM;
	}

	if (!m_pStaticBuffer[nSBID].bIndis) return ZFX_FAIL;

	if (m_dwActiveStaticBuffer != nSBID)
	{
		if (m_pStaticBuffer[nSBID].bIndis)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pStaticBuffer[nSBID].IndisBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_pStaticBuffer[nSBID].VertexBuffer);
			m_dwActiveStaticBuffer = nSBID;
		}
	}
	else if (m_dwActiveIndexBuffer != MAX_ID)
	{
		if (m_pStaticBuffer[nSBID].bIndis)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pStaticBuffer[nSBID].IndisBuffer);
		}
		m_dwActiveIndexBuffer = MAX_ID;
	}

	if (m_pOpenGL->GetActiveSkinID() != SkinID)
	{
		m_pOpenGL->ActiveSkin(SkinID);
	} // set device skin

	if (!m_pOpenGL->IsUseShaders())
	{
		// Set FVF
		ZFXOpenGLVCache::SetFVF(m_pStaticBuffer[nSBID].nVertexType);
	}

	if (m_pOpenGL->IsUseAdditiveBlending())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	if (!m_pOpenGL->IsUseColorBuffer())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE);
	}

	int nVertexNum = m_pStaticBuffer[nSBID].nVertexNum;
	int nIndisNum = m_pStaticBuffer[nSBID].nIndisNum;
	ZFXRENDERSTATE mode = m_pOpenGL->GetShadeMode();
	//// index primitive
	//// 没有实现StartIndex
	//switch (rs)
	//{
	//case RS_SHADE_POINTS:     // render just vertices
	//	glDrawArrays(GL_POINTS, 0, nVertexNum);
	//	break;
	//case RS_SHADE_LINES:      // render two verts as one line
	//	glDrawElements(GL_LINES, nIndisNum / 2, GL_UNSIGNED_SHORT, 0);
	//	break;
	//case RS_SHADE_HULLWIRE:   // render poly hull as polyline
	//	glDrawElements(GL_LINE_STRIP, nIndisNum, GL_UNSIGNED_SHORT, 0);
	//	break;
	//case RS_SHADE_TRIWIRE:    // render triangulated wire
	//case RS_SHADE_SOLID:	   // render solid polygons
	//default:
	//	glDrawElements(GL_TRIANGLES, nIndisNum / 3, GL_UNSIGNED_SHORT, 0);
	//	break;
	//}
	ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, true);

	hr = DrawPrimitive(mode, nVertexNum, nIndisNum, true);

	ZFXOpenGLVCache::SetClientStateEnable(m_pStaticBuffer[nSBID].nVertexType, false);

	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::RenderNaked(UINT nVerts, const void *pVerts, bool bTextured)
{
	HRESULT hr = ZFX_OK;
	InvalidateStates();
	m_pOpenGL->UseShaders(false);
	
	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	GLsizeiptr size = nVerts * sizeof(PVERTEX);
	glBufferData(GL_ARRAY_BUFFER, size, pVerts, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, sizeof(PVERTEX), 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, nVerts);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexbuffer);
	
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::RenderPoints(ZFXVERTEXID VertexID, UINT nVerts, const void *pVerts, const ZFXCOLOR *pClrl)
{
	InvalidateStates();
	
	if (pClrl)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pClrl->c);
	}
	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	int nStride = 0;
	// which vertex type is to be processed?
	switch (VertexID) 
	{
	case VID_PS: 
		nStride = sizeof(PVERTEX);
		glDisable(GL_LIGHTING);
		break;
	case VID_UU:
		nStride = sizeof(VERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_3T:
		nStride = sizeof(VERTEX3T);
		glEnable(GL_LIGHTING);
		break;
	case VID_TV:
		nStride = sizeof(TVERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_CA:
		nStride = sizeof(CVERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_UL: 
		nStride = sizeof(LVERTEX);
		glDisable(GL_LIGHTING);
		break;
	default:
		return ZFX_INVALIDID;
	} // switch

	

	GLuint vertexbuffer = 0;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	GLsizeiptr size = nVerts * nStride;
	glBufferData(GL_ARRAY_BUFFER, size, pVerts, GL_STATIC_DRAW);
	
	// shaders or FVF
	if (m_pOpenGL->IsUseShaders())
	{
		m_pOpenGL->ActivateVShader(0, VertexID);
		m_pOpenGL->ActivatePShader(0);
	}
	else
	{
		// Set FVF
		ZFXOpenGLVCache::SetFVF(VertexID);
	}

	ZFXOpenGLVCache::SetClientStateEnable(VertexID, true);

	glDrawArrays(GL_POINTS, 0, nVerts);

	ZFXOpenGLVCache::SetClientStateEnable(VertexID, false);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexbuffer);

	glEnable(GL_LIGHTING);
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::RenderLines(ZFXVERTEXID VertexID, UINT nVerts, const void *pVerts, const ZFXCOLOR *pClrl, bool bStrip)
{
	ForcedFlushAll();

	InvalidateStates();

	if (pClrl)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pClrl->c);
	}
	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	int nStride = 0;
	// which vertex type is to be processed?
	switch (VertexID)
	{
	case VID_PS:
		nStride = sizeof(PVERTEX);
		glDisable(GL_LIGHTING);
		break;
	case VID_UU:
		nStride = sizeof(VERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_3T:
		nStride = sizeof(VERTEX3T);
		glEnable(GL_LIGHTING);
		break;
	case VID_TV:
		nStride = sizeof(TVERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_CA:
		nStride = sizeof(CVERTEX);
		glEnable(GL_LIGHTING);
		break;
	case VID_UL:
		nStride = sizeof(LVERTEX);
		glDisable(GL_LIGHTING);
		break;
	default:
		return ZFX_INVALIDID;
	} // switch

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	GLsizeiptr size = nVerts * nStride;
	glBufferData(GL_ARRAY_BUFFER, size, pVerts, GL_STATIC_DRAW);
	
	// shaders or FVF
	if (m_pOpenGL->IsUseShaders())
	{
		m_pOpenGL->ActivateVShader(0, VertexID);
		m_pOpenGL->ActivatePShader(0);
	}
	else
	{
		// Set FVF
		ZFXOpenGLVCache::SetFVF(VertexID);
	}

	ZFXOpenGLVCache::SetClientStateEnable(VertexID, true);

	if (!bStrip)
	{
		glDrawArrays(GL_LINES, 0, nVerts);
	}
	else
	{
		glDrawArrays(GL_LINE_STRIP, 0, nVerts);
	}

	ZFXOpenGLVCache::SetClientStateEnable(VertexID, false);

	glEnable(GL_LIGHTING);
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::RenderLine(const float *fStart, const float *fEnd, const ZFXCOLOR *pClr)
{
	if (!pClr) return ZFX_INVALIDPARAM;

	ForcedFlushAll();

	InvalidateStates();

	m_pOpenGL->UseShaders(false);

	LVERTEX pVertex[2];
	pVertex[0].x = fStart[0];
	pVertex[0].y = fStart[1];
	pVertex[0].z = fStart[2];
	pVertex[1].x = fEnd[0];
	pVertex[1].y = fEnd[1];
	pVertex[1].z = fEnd[2];

	BYTE r = (BYTE)(pClr->fR * 255);
	BYTE g = (BYTE)(pClr->fG * 255);
	BYTE b = (BYTE)pClr->fB * 255;
	BYTE a = (BYTE)pClr->fA * 255;
	pVertex[0].Color = pVertex[1].Color = COLORDWORD(r, g, b, a);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pClr->c);

	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	m_pOpenGL->UseShaders(false);
	glEnable(GL_LIGHTING);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(LVERTEX), pVertex, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, sizeof(LVERTEX), 0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(LVERTEX), (void*)(3 * sizeof(float)));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexbuffer);

	glEnable(GL_LIGHTING);
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::ForcedFlushAll(void)
{
	bool bUseShader = m_pOpenGL->IsUseShaders();
	int i = 0;
	HRESULT hr = ZFX_OK;
	for (i = 0; i < ZFXOpenGLVCache_NUM; i++)
	{
		if (!m_pCachePS[i]->IsEmpty())
			if (FAILED(m_pCachePS[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;

		if (!m_pCacheUU[i]->IsEmpty())
			if (FAILED(m_pCacheUU[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;

		if (!m_pCacheUL[i]->IsEmpty())
			if (FAILED(m_pCacheUL[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;

		if (!m_pCacheCA[i]->IsEmpty())
			if (FAILED(m_pCacheCA[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;

		if (!m_pCache3T[i]->IsEmpty())
			if (FAILED(m_pCache3T[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;

		if (!m_pCacheTV[i]->IsEmpty())
			if (FAILED(m_pCacheTV[i]->Flush(bUseShader)))
				hr = ZFX_FAIL;
	}
	return hr;
}

HRESULT ZFXOpenGLVCacheManager::ForcedFlush(ZFXVERTEXID vid)
{
	ZFXOpenGLVCache** pCache = NULL;
	HRESULT hr = ZFX_OK;
	switch (vid)
	{
	case VID_PS: { pCache = m_pCachePS; } break;
	case VID_UU: { pCache = m_pCacheUU; } break;
	case VID_UL: { pCache = m_pCacheUL; } break;
	case VID_CA: { pCache = m_pCacheCA; } break;
	case VID_3T: { pCache = m_pCache3T; } break;
	case VID_TV: { pCache = m_pCacheTV; } break;

		// unknown vertex type
	default: return ZFX_INVALIDID;
	} // switch

	for (int i = 0; i < ZFXOpenGLVCache_NUM; i++)
	{
		if (!pCache[i]->IsEmpty())
		{
			if (FAILED(pCache[i]->Flush(m_pOpenGL->IsUseShaders())))
				hr = ZFX_FAIL;
		}
	}

	return hr;
}

void ZFXOpenGLVCacheManager::InvalidateStates(void)
{
	m_pOpenGL->SetActiveSkinID(MAX_ID);
	m_dwActiveStaticBuffer = MAX_ID;
	m_dwActiveIndexBuffer = MAX_ID;
	// m_dwActiveVCache = MAX_ID;
}

ZFXRENDERSTATE ZFXOpenGLVCacheManager::GetShadeMode(void)
{
	if (m_pOpenGL)
		return m_pOpenGL->GetShadeMode();
	else return RS_NONE;
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

HRESULT ZFXOpenGLVCacheManager::DrawPrimitive(ZFXRENDERSTATE mode, int nVertexNum, int nIndisNum, bool bIndis)
{
	if (nVertexNum == 0 && nIndisNum == 0)
	{
		Log("Error DrawPrimitive nVertexNum and nIndisNum equal 0");
		return ZFX_INVALIDPARAM;
	}

	HRESULT hr = ZFX_OK;
	if (bIndis)
	{
		// index primitive
		switch (mode)
		{
		case RS_SHADE_POINTS:     // render just vertices
			glDrawArrays(GL_POINTS, 0, nVertexNum);
			break;
		case RS_SHADE_LINES:      // render two verts as one line
			glDrawElements(GL_LINES, nIndisNum, GL_UNSIGNED_SHORT, 0);
			break;
		case RS_SHADE_HULLWIRE:   // render poly hull as polyline
			glDrawElements(GL_LINE_STRIP, nIndisNum, GL_UNSIGNED_SHORT, 0);
			break;
		case RS_SHADE_QUADS:
			glDrawElements(GL_QUADS, nIndisNum, GL_UNSIGNED_SHORT, 0);
			break;
		case RS_SHADE_TRIWIRE:    // render triangulated wire
		case RS_SHADE_SOLID:	   // render solid polygons
		default:
			// count 是索引点的数量，不是三角形的数量
			glDrawElements(GL_TRIANGLES, nIndisNum, GL_UNSIGNED_SHORT, 0);
			break;
		}
	}
	else
	{
		// non-index primitive
		switch (mode)
		{
		case RS_SHADE_POINTS:     // render just vertices
			glDrawArrays(GL_POINTS, 0, nVertexNum);
			break;
		case RS_SHADE_LINES:      // render two verts as one line
			glDrawArrays(GL_LINES, 0, nVertexNum);
			break;
		case RS_SHADE_HULLWIRE:   // render poly hull as polyline
			glDrawArrays(GL_LINE_STRIP, 0, nVertexNum);
			break;
		case RS_SHADE_QUADS:
			glDrawArrays(GL_QUADS, 0, nIndisNum);
			break;
		case RS_SHADE_TRIWIRE:    // render triangulated wire
		case RS_SHADE_SOLID:	   // render solid polygons
		default:
			glDrawArrays(GL_TRIANGLES, 0, nVertexNum);
			break;
		}
	}
	CHECK_ERROR_RETURN(hr);
	return hr;
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
	memset(m_Skin.nTexture, ZFXOpenGLSkinManager::MAX_ID, 8 * sizeof(UINT));
	m_SkinID = ZFXOpenGLSkinManager::MAX_ID;

	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	GLsizeiptr size = nVertsMax * nStride;
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	
	//SetFVF(vid);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	size = nIndisMax * sizeof(WORD);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

ZFXOpenGLVCache::~ZFXOpenGLVCache()
{
	if (glIsBuffer(m_VertexBuffer))
		glDeleteBuffers(1, &m_VertexBuffer);

	if (glIsBuffer(m_IndexBuffer))
		glDeleteBuffers(1, &m_IndexBuffer);
}

HRESULT ZFXOpenGLVCache::Flush(bool bUseShaders)
{
	if (m_nVertexNum <= 0) return ZFX_OK;
	if (!m_pVCMan) return ZFX_FAIL;

	if (m_pVCMan->GetActiveVCache() != m_dwID)
	{
		if (!bUseShaders)
		{
			SetFVF(m_vid);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		m_pVCMan->SetActiveVCache(m_dwID);
	}

	if (m_pVCMan->GetOpenGL()->GetActiveSkinID() != m_SkinID)
	{
		m_pVCMan->GetOpenGL()->ActiveSkin(m_SkinID);
	} // set device skin


	if (m_pVCMan->GetOpenGL()->IsUseAdditiveBlending())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}

	if (!m_pVCMan->GetOpenGL()->IsUseColorBuffer())
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE);
	}

	SetClientStateEnable(m_vid, true);

	ZFXRENDERSTATE mode = m_pVCMan->GetOpenGL()->GetShadeMode();
	m_pVCMan->DrawPrimitive(mode, m_nVertexNum, m_nIndisNum, true);

	SetClientStateEnable(m_vid, false);
	
	m_nIndisNum = 0;
	m_nVertexNum = 0;
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCache::Add(UINT nVerts, UINT nIndis, const void *pVerts, const WORD *pIndis, bool bUseShaders)
{
	if (nVerts == 0 || nIndis == 0)
		return E_INVALIDARG;

	if (pVerts == NULL || pIndis == NULL)
		return E_INVALIDARG;

	if (nVerts > m_nMaxVertexNum || nIndis > m_nMaxIndisNum)
		return ZFX_BUFFERSIZE;

	if ((nVerts + m_nVertexNum) > m_nMaxVertexNum ||
		(nIndis + m_nIndisNum) > m_nMaxIndisNum)
	{
		if (Flush(bUseShaders) != ZFX_OK)
		{
			Log("VCache Flush error");
			return ZFX_FAIL;
		}
	}

	if (!glIsBuffer(m_VertexBuffer))
	{
		Log("VCache Invalid Vertex Buffer");
		return ZFX_FAIL;
	}
	GLintptr offset = m_nVertexNum * m_nStride;
	GLsizeiptr size = nVerts * m_nStride;
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, pVerts);

	if (!glIsBuffer(m_IndexBuffer))
	{
		Log("VCache Invalid Index Buffer");
		return ZFX_FAIL;
	}
	offset = m_nVertexNum * sizeof(WORD);
	size = nIndis * sizeof(WORD);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, pIndis);

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

HRESULT ZFXOpenGLVCache::SetFVF(ZFXVERTEXID vid)
{
	HRESULT hr = ZFX_OK;
	switch (vid)
	{
	case VID_PS:
		glVertexPointer(3, GL_FLOAT, sizeof(PVERTEX), 0);
		break;
	case VID_UU:
		glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 3));
		//glClientActiveTexture(GL_TEXTURE0);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_UL:
		glVertexPointer(3, GL_FLOAT, sizeof(LVERTEX), 0);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(LVERTEX), (void*)(sizeof(float) * 3));
		/*glClientActiveTexture(GL_TEXTURE0);*/
		glTexCoordPointer(2, GL_FLOAT, sizeof(LVERTEX), (void*)(sizeof(float) * 3 + sizeof(DWORD)));
		break;
	case VID_CA:
		glVertexPointer(3, GL_FLOAT, sizeof(CVERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 3));
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_3T:
		glVertexPointer(3, GL_FLOAT, sizeof(VERTEX3T), 0);
		glNormalPointer(GL_FLOAT, sizeof(VERTEX3T), (void*)(sizeof(float) * 3));
		
		glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX3T), (void*)(sizeof(float) * 6));
		break;
	case VID_TV:
		glVertexPointer(3, GL_FLOAT, sizeof(TVERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(TVERTEX), (void*)(sizeof(float) * 3));
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(TVERTEX), (void*)(sizeof(float) * 6));
		break;
	default:
		hr = E_INVALIDARG;
		break;
	}
	return hr;
}

HRESULT ZFXOpenGLVCache::SetClientStateEnable(ZFXVERTEXID vid, bool bEnable)
{
	if (bEnable)
	{
		switch (vid)
		{
		case VID_PS:
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case VID_UL:
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		case VID_UU:		
		case VID_CA:			
		case VID_3T:
		case VID_TV:
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			//glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (vid)
		{
		case VID_PS:
			glDisableClientState(GL_VERTEX_ARRAY);
			break;
		case VID_UL:
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		case VID_UU:
		case VID_CA:
		case VID_3T:
		case VID_TV:
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
		default:
			break;
		}
	}
	
	return ZFX_OK;
}

