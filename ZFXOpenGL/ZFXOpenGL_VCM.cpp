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

