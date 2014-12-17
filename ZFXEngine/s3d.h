#ifndef _S3D_H_
#define _S3D_H_

#include "ZFX.h"
#include "ZFXRenderDevice.h"

typedef struct TRI_TYPE
{
	WORD i0, i1, i2;
	WORD n0, n1, n2;
	UINT nMat;
}TRI;

typedef int(*CMPFUNC) (const void *arg1, const void *arg2);
int SortTris(const TRI *arg1, const TRI* arg2);

class ZFXModel
{
protected:
	ZFXRenderDevice* m_pRenderDevice;

	UINT m_nSkinNum;
	UINT *m_pSkin;

	UINT m_nVertexNum;
	VERTEX *m_pVertex;

	VERTEX3T *m_pVertex3T;
	UINT *m_pBufferID3T;

	UINT m_nIndexNum;
	WORD *m_pIndex;

	UINT *m_pCount;
	UINT *m_pBufferID;

	FILE *m_pFile;

	bool m_bReady;

	void ReadFile();

public:
	ZFXModel(const char * chFile, ZFXRenderDevice *pDevice);
	~ZFXModel();

	HRESULT Render(bool bStatic, bool b3T);
	void Log(const char *fmt, ...);

};

#define SEEK(str, key) while (instr(str,key)==-1) fgets(str,80,m_pFile);
#define NEXT(str) fgets(str, 80, m_pFile);
int instr(const char *string, const char *substring);

#endif