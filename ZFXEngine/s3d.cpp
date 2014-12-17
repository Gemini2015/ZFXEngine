#include "s3d.h"

ZFXModel::ZFXModel(const char * chFile, ZFXRenderDevice *pDevice)
{
	m_nSkinNum = 0;
	m_nVertexNum = 0;
	m_nIndexNum = 0;

	m_pVertex3T = NULL;
	m_pVertex = NULL;
	m_pIndex = NULL;
	m_pSkin = NULL;
	m_pBufferID = NULL;
	m_pBufferID3T = NULL;
	m_pCount = NULL;

	m_pRenderDevice = pDevice;
	m_bReady = false;

	m_pFile = fopen(chFile, "r");

	if (m_pFile)
	{
		ReadFile();
		fclose(m_pFile);
		m_bReady = true;
	}
	else
	{
		Log("Error Model Open File ");
	}
}

ZFXModel::~ZFXModel()
{
	if (m_pIndex)
	{
		delete[] m_pIndex;
		m_pIndex = NULL;
	}
	if (m_pVertex)
	{
		delete[] m_pVertex;
		m_pVertex = NULL;
	}
	if (m_pVertex3T)
	{
		delete[] m_pVertex3T;
		m_pVertex3T = NULL;
	}
	if (m_pSkin)
	{
		delete[] m_pSkin;
		m_pSkin = NULL;
	}
	if (m_pCount)
	{
		delete[] m_pCount;
		m_pCount = NULL;
	}
	if (m_pBufferID)
	{
		delete[] m_pBufferID;
		m_pBufferID = NULL;
	}
	if (m_pBufferID3T)
	{
		delete[] m_pBufferID3T;
		m_pBufferID3T = NULL;
	}

	m_bReady = false;
}

HRESULT ZFXModel::Render(bool bStatic, bool b3T)
{
	HRESULT hr = ZFX_OK;
	m_pRenderDevice->SetAmbientLight(1.0f, 1.0f, 1.0f);

	if (bStatic)
	{
		for (int i = 0; i < m_nSkinNum; i++)
		{
			if (b3T)
			{
				if (FAILED(m_pRenderDevice->GetVertexManager()->Render(m_pBufferID3T[i])))
					return ZFX_FAIL;
			}
			else
			{
				if (FAILED(m_pRenderDevice->GetVertexManager()->Render(m_pBufferID[i])))
					return ZFX_FAIL;
			}
		}
	}
	else
	{
		WORD *pIndex = m_pIndex;
		for (int i = 0; i < m_nSkinNum; pIndex += m_pCount[i], i++)
		{
			if (b3T)
			{
				if (FAILED(m_pRenderDevice->GetVertexManager()->Render(VID_3T,
					m_nVertexNum, m_pCount[i], m_pVertex3T, pIndex, m_pSkin[i])))
					hr = ZFX_FAIL;
			}
			else
			{
				if (FAILED(m_pRenderDevice->GetVertexManager()->Render(VID_UU,
					m_nVertexNum, m_pCount[i], m_pVertex, pIndex, m_pSkin[i])))
					hr = ZFX_FAIL;
			}
		}
	}

	return hr;
}

void ZFXModel::Log(const char *fmt, ...)
{
	char ch[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf_s(ch, fmt, args);
	va_end(args);

	GetLogger().Print(ch);
}

void ZFXModel::ReadFile()
{
	ZFXCOLOR cA, cD, cE, cS;
	UINT     i, nNumFaces, nNumTris = 0;
	float    fPower = 0;
	char     Line[80];
	char     Texture[80];
	TRI     *pTris = NULL;


	// READ SKINS
	SEEK(Line, "BEGIN_SKINS");
	NEXT(Line);
	sscanf(Line, "%d;", &m_nSkinNum);
	m_pSkin = new UINT[m_nSkinNum];
	for (i = 0; i < m_nSkinNum; i++) 
	{
		NEXT(Line);  NEXT(Line);// skip opening brag
		sscanf(Line, "%f, %f, %f, %f;", &cA.fR, &cA.fG, &cA.fB, &cA.fA);           NEXT(Line);
		sscanf(Line, "%f,%f,%f,%f;", &cD.fR, &cD.fG, &cD.fB, &cD.fA);              NEXT(Line);
		sscanf(Line, "%f,%f,%f,%f;", &cE.fR, &cE.fG, &cE.fB, &cE.fA);              NEXT(Line);
		sscanf(Line, "%f,%f,%f,%f,%f;", &cS.fR, &cS.fG, &cS.fB, &cS.fA, &fPower);  NEXT(Line);
		sscanf(Line, "%s", &Texture);

		// add skin to skin-manager
		m_pRenderDevice->GetSkinManager()->AddSkin(&cA, &cD, &cE, &cS, fPower, &m_pSkin[i]);
		m_pRenderDevice->GetSkinManager()->AddTexture(m_pSkin[i], Texture, false, 0, NULL, 0);
		NEXT(Line); // skip closing brag
	}


	// READ VERTICES
	rewind(m_pFile);
	SEEK(Line, "BEGIN_VERTICES");
	NEXT(Line);
	sscanf(Line, "%d;", &m_nVertexNum);
	m_pVertex = new VERTEX[m_nVertexNum];
	m_pVertex3T = new VERTEX3T[m_nVertexNum];
	for (i = 0; i < m_nVertexNum; i++) 
	{
		NEXT(Line);
		sscanf(Line, "%f,%f,%f,%f,%f;", &m_pVertex[i].x,
			&m_pVertex[i].y, &m_pVertex[i].z,
			&m_pVertex[i].tu, &m_pVertex[i].tv);

		memset(&m_pVertex3T[i], 0, sizeof(VERTEX3T));
		m_pVertex3T[i].x = m_pVertex[i].x;
		m_pVertex3T[i].y = m_pVertex[i].y;
		m_pVertex3T[i].z = m_pVertex[i].z;
		m_pVertex3T[i].tu0 = m_pVertex[i].tu;
		m_pVertex3T[i].tv0 = m_pVertex[i].tv;
	}


	// READ FACES (n-sided polys) TO COUNT TRIS NEEDED
	rewind(m_pFile);
	SEEK(Line, "BEGIN_FACES");
	NEXT(Line);
	sscanf(Line, "%d;", &nNumFaces);
	pTris = new TRI[nNumFaces];
	for (i = 0; i < nNumFaces; i++) 
	{
		NEXT(Line);
		sscanf(Line, "%d,%d,%d;%d", &pTris[nNumTris].i0,
			&pTris[nNumTris].i1, &pTris[nNumTris].i2,
			&pTris[nNumTris].nMat);
		nNumTris++;
	}

	// sort tris by material
	qsort((void*)pTris, nNumTris, sizeof(TRI), (CMPFUNC)SortTris);



	// COUNT INDICES FOR EACH MATERIAL
	UINT nOldMat = pTris[0].nMat;
	m_pCount = new UINT[m_nSkinNum];
	m_pBufferID = new UINT[m_nSkinNum];
	m_pBufferID3T = new UINT[m_nSkinNum];
	memset(m_pCount, 0, sizeof(UINT)*m_nSkinNum);
	memset(m_pBufferID, 0, sizeof(UINT)*m_nSkinNum);
	memset(m_pBufferID3T, 0, sizeof(UINT)*m_nSkinNum);
	m_pIndex = new WORD[nNumTris * 3];
	m_nIndexNum = nNumFaces * 3;
	for (i = 0; i < nNumTris; i++) 
	{
		// copy indices in indexlist
		m_pIndex[i * 3] = pTris[i].i0;
		m_pIndex[i * 3 + 1] = pTris[i].i1;
		m_pIndex[i * 3 + 2] = pTris[i].i2;

		// count indices per material
		if (pTris[i].nMat != nOldMat)
			nOldMat = pTris[i].nMat;
		else
			m_pCount[pTris[i].nMat] += 3;
	}


	WORD *pIndices = m_pIndex;
	for (i = 0; i < m_nSkinNum; pIndices += m_pCount[i], i++)
	{
		if (FAILED(m_pRenderDevice->GetVertexManager()->CreateStaticBuffer(
			VID_UU,
			m_pSkin[i],
			m_nVertexNum,
			m_pCount[i],
			m_pVertex,
			m_pIndex,
			&m_pBufferID[i])))
			Log("CREATE STATIC FAILED\n");

		if (FAILED(m_pRenderDevice->GetVertexManager()->CreateStaticBuffer(
			VID_3T,
			m_pSkin[i],
			m_nVertexNum,
			m_pCount[i],
			m_pVertex3T,
			m_pIndex,
			&m_pBufferID3T[i])))
			Log("CREATE STATIC 3T FAILED\n");

	}
	delete[] pTris;
}

int SortTris(const TRI *arg1, const TRI *arg2) 
{
	// Tausch der Rückgabewerte 1 bzw -1 kehrt die Sortier-
	// reihenfolge um. So beginnen wir mit dem kleinsten:
	if (arg1->nMat > arg2->nMat)
		return 1;
	else if (arg1->nMat < arg2->nMat)
		return -1;
	else
		return 0;
} // SortTris

/**
* Test if a string contains the substring. Returns first appearence
* of substring (also 0) or -1 else.
*/
int instr(const char *string, const char *substring) 
{
	char a, c;
	int  nStart;
	int  nLng_SubStr = strlen(substring),
		nLng_Str = strlen(string);

	// FALSE wenn Substring groesser als String
	if ((nLng_SubStr <= 1) || (nLng_SubStr > nLng_Str))
		return -1;

	// Suche ersten Substring-Buchstaben im String
	memcpy(&a, &substring[0], sizeof(char));
	nStart = strcspn(string, &a);

	// Suche die folgenden Buchstaben des Substrings
	while (nStart < nLng_Str) 
	{
		// Start im Str ist nicht der erste Substr-Buchstb
		if (string[nStart] != a) 
		{
			nStart++;
			continue;
		}
		// Vergleiche die dem Start folgenden Buchstb im Str 
		int j = 0;
		for (j = 1; j<nLng_SubStr; j++) 
		{
			memcpy(&c, &substring[j], sizeof(char));
			if (string[nStart + j] != c) 
			{
				break;
			}
		} // for [nLng_SubStr]
		// Waren alle Substr-Buchstb Nachfolger von Start im Str
		// dann haben wir den Substring im String gefunden.
		if (j == nLng_SubStr)
			return nStart;
		// Sonst verschieben wir die Startposition im Str um eins
		else
			nStart++;
	} // while
	// Ganzen String durchsucht und nichts gefunden
	return -1;
} // instr