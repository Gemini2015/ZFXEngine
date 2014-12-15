#include "ZFXOpenGL_VCM.h"


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
		GLsizeiptr size = nIndis * sizeof(WORD);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pIndis, GL_STATIC_DRAW);
		glIndexPointer(GL_UNSIGNED_SHORT, sizeof(WORD), 0);

		m_pStaticBuffer[m_nStaticBufferNum].IndisBuffer = indexbuffer;
	}
	else
	{
		m_pStaticBuffer[m_nStaticBufferNum].bIndis = false;
		m_pStaticBuffer[m_nStaticBufferNum].nTriangleNum = (int)(nVerts / 3.0f);
		m_pStaticBuffer[m_nStaticBufferNum].IndisBuffer = 0;
	}

	ZFXVERTEXID vid = VID_NONE;
	if (m_pOpenGL && m_pOpenGL->IsUseShaders()) vid = VID_NONE;
	else vid = m_pStaticBuffer[m_nStaticBufferNum].nVertexType;

	GLuint vertexbuffer = 0;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
	GLsizeiptr size = nVerts * m_pStaticBuffer[m_nStaticBufferNum].nStride;
	glBufferData(GL_VERTEX_ARRAY, size, pVerts, GL_STATIC_DRAW);
	
	// Set FVF

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
	glIndexPointer(GL_UNSIGNED_SHORT, sizeof(WORD), 0);

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

	m_dwActiveVCache = MAX_ID;

	if (nSBID >= MAX_ID)
	{
		Log("Error Invalid Static Buffer ID");
		return ZFX_INVALIDPARAM;
	}

	if (m_dwActiveStaticBuffer != nSBID)
	{
		if (m_pStaticBuffer[nSBID].bIndis)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pStaticBuffer[nSBID].IndisBuffer);

			glBindBuffer(GL_VERTEX_ARRAY, m_pStaticBuffer[nSBID].VertexBuffer);
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

	if (m_pOpenGL->GetActiveSkinID() != m_pStaticBuffer[nSBID].nSkinID)
	{
		GLuint texture = 0;
		ZFXSKIN skin = m_pSkinMan->GetSkin(m_pStaticBuffer[nSBID].nSkinID);
		ZFXMATERIAL *pMat = &m_pSkinMan->GetMaterial(skin.nMaterial);

		if (m_pOpenGL->GetShadeMode() == RS_SHADE_SOLID)
		{
			if (!m_pOpenGL->IsUseShaders())
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMat->cDiffuse.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMat->cAmbient.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMat->cSpecular.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pMat->cEmissive.c);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMat->fPower);

			}
			else
			{
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 1, 1, &pMat->cAmbient);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 2, 1, &pMat->cDiffuse);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 3, 1, &pMat->cEmissive);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 4, 1, &pMat->cSpecular);
			}

			if (m_pOpenGL->IsUseTextures())
			{
				for (int i = 0; i < 8; i++)
				{
					if (skin.nTexture[i] != ZFXOpenGLSkinManager::MAX_ID)
					{
						ZFXTEXTURE zfxtexture = m_pSkinMan->GetTexture(skin.nTexture[i]);
						if (zfxtexture.pData == NULL)
							continue;

						GLuint texture = *(GLuint*)zfxtexture.pData;
						glActiveTexture(GL_TEXTURE0 + i);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, texture);
						glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_EXT, m_pOpenGL->GetTextureOp(i));

					}
					else break;
				}
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE0);
				glDisable(GL_TEXTURE_2D);
			}
		}
		else // not solid 
		{
			ZFXCOLOR clrWire = m_pOpenGL->GetWireColor();
			GLfloat clr[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clrWire.c);
			//specular and emission default 0,0,0,1

			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);

		}

		if (skin.bAlpha)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GEQUAL, 50 / 255.0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}
		m_pOpenGL->SetActiveSkinID(m_pStaticBuffer[nSBID].nSkinID);
	} // set device skin

	if (!m_pOpenGL->IsUseShaders())
	{
		// Set FVF
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

	ZFXRENDERSTATE rs = m_pOpenGL->GetShadeMode();

	int nVertexNum = m_pStaticBuffer[nSBID].nVertexNum;
	int nIndisNum = m_pStaticBuffer[nSBID].nIndisNum;
	if (m_pStaticBuffer[nSBID].bIndis)
	{
		// index primitive
		switch (rs)
		{
		case RS_SHADE_POINTS:     // render just vertices
			glDrawArrays(GL_POINTS, 0, nVertexNum);
			break;
		case RS_SHADE_LINES:      // render two verts as one line
			glDrawElements(GL_LINES, nIndisNum / 2, GL_UNSIGNED_SHORT, 0);
			break;
		case RS_SHADE_HULLWIRE:   // render poly hull as polyline
			glDrawElements(GL_LINE_STRIP, nIndisNum, GL_UNSIGNED_SHORT, 0);
			break;
		case RS_SHADE_TRIWIRE:    // render triangulated wire
		case RS_SHADE_SOLID:	   // render solid polygons
		default:
			glDrawElements(GL_TRIANGLES, nIndisNum / 3, GL_UNSIGNED_SHORT, 0);
			break;
		}
	}
	else
	{
		// non-index primitive
		switch (rs)
		{
		case RS_SHADE_POINTS:     // render just vertices
			glDrawArrays(GL_POINTS, 0, nVertexNum);
			break;
		case RS_SHADE_LINES:      // render two verts as one line
			glDrawArrays(GL_LINES, 0, nVertexNum / 2);
			break;
		case RS_SHADE_HULLWIRE:   // render poly hull as polyline
			glDrawArrays(GL_LINE_STRIP, 0, nVertexNum);
			break;
		case RS_SHADE_TRIWIRE:    // render triangulated wire
		case RS_SHADE_SOLID:	   // render solid polygons
		default:
			glDrawArrays(GL_TRIANGLES, 0, m_pStaticBuffer[nSBID].nTriangleNum);
			break;
		}
	}

	return ZFX_OK;

}

HRESULT ZFXOpenGLVCacheManager::Render(UINT SBID, UINT IBID, UINT Skin)
{
	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::Render(UINT nSBID, UINT SkinID, UINT StartIndex, UINT NumVerts, UINT NumTrims)
{
	HRESULT hr = ZFX_OK;

	ZFXRENDERSTATE rs = m_pOpenGL->GetShadeMode();

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
			glBindBuffer(GL_VERTEX_ARRAY, m_pStaticBuffer[nSBID].VertexBuffer);
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
		GLuint texture = 0;
		ZFXSKIN skin = m_pSkinMan->GetSkin(SkinID);
		ZFXMATERIAL *pMat = &m_pSkinMan->GetMaterial(skin.nMaterial);

		if (m_pOpenGL->GetShadeMode() == RS_SHADE_SOLID)
		{
			if (!m_pOpenGL->IsUseShaders())
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMat->cDiffuse.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMat->cAmbient.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMat->cSpecular.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pMat->cEmissive.c);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMat->fPower);

			}
			else
			{
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 1, 1, &pMat->cAmbient);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 2, 1, &pMat->cDiffuse);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 3, 1, &pMat->cEmissive);
				m_pOpenGL->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 4, 1, &pMat->cSpecular);
			}

			if (m_pOpenGL->IsUseTextures())
			{
				for (int i = 0; i < 8; i++)
				{
					if (skin.nTexture[i] != ZFXOpenGLSkinManager::MAX_ID)
					{
						ZFXTEXTURE zfxtexture = m_pSkinMan->GetTexture(skin.nTexture[i]);
						if (zfxtexture.pData == NULL)
							continue;

						GLuint texture = *(GLuint*)zfxtexture.pData;
						glActiveTexture(GL_TEXTURE0 + i);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, texture);
						glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_EXT, m_pOpenGL->GetTextureOp(i));

					}
					else break;
				}
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE0);
				glDisable(GL_TEXTURE_2D);
			}
		}
		else // not solid 
		{
			ZFXCOLOR clrWire = m_pOpenGL->GetWireColor();
			GLfloat clr[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clrWire.c);
			//specular and emission default 0,0,0,1

			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);

		}

		if (skin.bAlpha)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GEQUAL, 50 / 255.0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}
		m_pOpenGL->SetActiveSkinID(m_pStaticBuffer[nSBID].nSkinID);
	} // set device skin

	if (!m_pOpenGL->IsUseShaders())
	{
		// Set FVF
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
	// index primitive
	switch (rs)
	{
	case RS_SHADE_POINTS:     // render just vertices
		glDrawArrays(GL_POINTS, 0, nVertexNum);
		break;
	case RS_SHADE_LINES:      // render two verts as one line
	
		glDrawElements(GL_LINES, nIndisNum / 2, GL_UNSIGNED_SHORT, 0);
		break;
	case RS_SHADE_HULLWIRE:   // render poly hull as polyline
		glDrawElements(GL_LINE_STRIP, nIndisNum, GL_UNSIGNED_SHORT, 0);
		break;
	case RS_SHADE_TRIWIRE:    // render triangulated wire
	case RS_SHADE_SOLID:	   // render solid polygons
	default:
		glDrawElements(GL_TRIANGLES, nIndisNum / 3, GL_UNSIGNED_SHORT, 0);
		break;
	}

	return ZFX_OK;
}

HRESULT ZFXOpenGLVCacheManager::RenderNaked(UINT nVerts, const void *pVerts, bool bTextured)
{
	HRESULT hr = ZFX_OK;
	InvalidateStates();
	m_pOpenGL->UseShaders(false);
	
	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	
	// Set FVF

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
	GLsizeiptr size = nVerts * sizeof(PVERTEX);
	glBufferData(GL_VERTEX_ARRAY, size, pVerts, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, sizeof(PVERTEX), 0);

	glDrawArrays(GL_TRIANGLES, 0, nVerts / 3);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
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
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
	GLsizeiptr size = nVerts * nStride;
	glBufferData(GL_VERTEX_ARRAY, size, pVerts, GL_STATIC_DRAW);
	
	// shaders or FVF
	if (m_pOpenGL->IsUseShaders())
	{
		m_pOpenGL->ActivateVShader(0, VertexID);
		m_pOpenGL->ActivatePShader(0);
	}
	else
	{
		// Set FVF
	}

	glDrawArrays(GL_POINTS, 0, nVerts);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
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
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
	GLsizeiptr size = nVerts * nStride;
	glBufferData(GL_VERTEX_ARRAY, size, pVerts, GL_STATIC_DRAW);
	
	// shaders or FVF
	if (m_pOpenGL->IsUseShaders())
	{
		m_pOpenGL->ActivateVShader(0, VertexID);
		m_pOpenGL->ActivatePShader(0);
	}
	else
	{
		// Set FVF
	}

	if (!bStrip)
	{
		glDrawArrays(GL_LINES, 0, nVerts / 2);
	}
	else
	{
		glDrawArrays(GL_LINE_STRIP, 0, nVerts);
	}

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

	BYTE r = (pClr->fR * 255);
	BYTE g = (pClr->fG * 255);
	BYTE b = pClr->fB * 255;
	BYTE a = pClr->fA * 255;
	pVertex[0].Color = pVertex[1].Color = COLORDWORD(r, g, b, a);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pClr->c);

	glClientActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	m_pOpenGL->UseShaders(false);
	glEnable(GL_LIGHTING);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_VERTEX_ARRAY, vertexbuffer);
	glBufferData(GL_VERTEX_ARRAY, 2 * sizeof(LVERTEX), pVertex, GL_STATIC_DRAW);

	
	glVertexPointer(3, GL_FLOAT, sizeof(LVERTEX), 0);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(LVERTEX), (void*)(3 * sizeof(float)));
	
	glDrawArrays(GL_LINES, 0, 1);

	glBindBuffer(GL_VERTEX_ARRAY, 0);
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
	else RS_NONE;
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
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_UL:
		glVertexPointer(3, GL_FLOAT, sizeof(LVERTEX), 0);
		glColorPointer(4, GL_UNSIGNED_INT, sizeof(LVERTEX), (void*)(sizeof(float) * 3));
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(LVERTEX), (void*)(sizeof(float) * 3 + sizeof(DWORD)));
		break;
	case VID_CA:
		glVertexPointer(3, GL_FLOAT, sizeof(CVERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 3));
		glClientActiveTexture(GL_TEXTURE0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CVERTEX), (void*)(sizeof(float) * 6));
		break;
	case VID_3T:
		glVertexPointer(3, GL_FLOAT, sizeof(VERTEX), 0);
		glNormalPointer(GL_FLOAT, sizeof(VERTEX), (void*)(sizeof(float) * 3));
		glClientActiveTexture(GL_TEXTURE0);
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
	HRESULT hr;
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
		GLuint texture = 0;
		ZFXMATERIAL *pMat = &m_pSkinMan->GetMaterial(m_Skin.nMaterial);

		if (m_pVCMan->GetOpenGL()->GetShadeMode() == RS_SHADE_SOLID)
		{
			if (!m_pVCMan->GetOpenGL()->IsUseShaders())
			{
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, pMat->cDiffuse.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pMat->cAmbient.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pMat->cSpecular.c);
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pMat->cEmissive.c);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, pMat->fPower);

			}
			else
			{
				m_pVCMan->GetOpenGL()->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 1, 1, &pMat->cAmbient);
				m_pVCMan->GetOpenGL()->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 2, 1, &pMat->cDiffuse);
				m_pVCMan->GetOpenGL()->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 3, 1, &pMat->cEmissive);
				m_pVCMan->GetOpenGL()->SetShaderConstant(SHT_PIXEL, DAT_FLOAT, 4, 1, &pMat->cSpecular);
			}

			if (m_pVCMan->GetOpenGL()->IsUseTextures())
			{
				for (int i = 0; i < 8; i++)
				{
					if (m_Skin.nTexture[i] != ZFXOpenGLSkinManager::MAX_ID)
					{
						ZFXTEXTURE zfxtexture = m_pSkinMan->GetTexture(m_Skin.nTexture[i]);
						if (zfxtexture.pData == NULL)
							continue;

						GLuint texture = *(GLuint*)zfxtexture.pData;
						glActiveTexture(GL_TEXTURE0 + i);
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, texture);
						glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
						glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_EXT, m_pVCMan->GetOpenGL()->GetTextureOp(i));

					}
					else break;
				}
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
				glActiveTexture(GL_TEXTURE0);
				glDisable(GL_TEXTURE_2D);
			}
		}
		else // not solid 
		{
			ZFXCOLOR clrWire = m_pVCMan->GetOpenGL()->GetWireColor();
			GLfloat clr[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, clrWire.c);
			//specular and emission default 0,0,0,1

			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);

		}

		if (m_Skin.bAlpha)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GEQUAL, 50 / 255.0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}
		m_pVCMan->GetOpenGL()->SetActiveSkinID(m_SkinID);
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

	ZFXRENDERSTATE rs = m_pVCMan->GetOpenGL()->GetShadeMode();

	switch (rs)
	{
	case RS_SHADE_POINTS:     // render just vertices
		glDrawArrays(GL_POINTS, 0, m_nVertexNum);
		break;
	case RS_SHADE_LINES:      // render two verts as one line
		glDrawElements(GL_LINES, m_nIndisNum / 2, GL_UNSIGNED_SHORT, 0);
		break;
	case RS_SHADE_HULLWIRE:   // render poly hull as polyline
		glDrawElements(GL_LINE_STRIP, m_nIndisNum, GL_UNSIGNED_SHORT, 0);
		break;
	case RS_SHADE_TRIWIRE:    // render triangulated wire
	case RS_SHADE_SOLID:	   // render solid polygons
	default:
		glDrawElements(GL_TRIANGLES, m_nIndisNum / 3, GL_UNSIGNED_SHORT, 0);
		break;
	}
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

