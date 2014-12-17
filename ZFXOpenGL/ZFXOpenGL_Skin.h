#ifndef _ZFXOPENGL_SKIN_H_
#define _ZFXOPENGL_SKIN_H_


#include "ZFXRenderDevice.h"
#include "ZFX.h"
#include "gl\GL.h"

#define COLORDWORD(r,g,b,a) ((DWORD)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))

#define CHECK_ERROR {\
	GLenum error = glGetError();\
	if(error != GL_NO_ERROR)\
		 Log("OpenGL Error > File:%s Line:%d Error:%d",__FILE__, __LINE__, error);\
}

class ZFXOpenGLSkinManager : public ZFXSkinManager
{
public:
	enum OpenGLSkinManager_Enum
	{
		MAX_ID = 65535,
	};

	ZFXOpenGLSkinManager();
	~ZFXOpenGLSkinManager();

	virtual void Reset(void) override;

	virtual HRESULT AddSkin(const ZFXCOLOR *pcAmbient, 
		const ZFXCOLOR *pcDiffuse, 
		const ZFXCOLOR *pcEmissive, 
		const ZFXCOLOR *pcSpecular, 
		float fSpecPower, 
		UINT *nSkinID) override;

	virtual HRESULT AddTexture(UINT nSkinID, 
		const char *chName, 
		bool bAlpha, 
		float fAlpha, 
		ZFXCOLOR *cColorKeys, 
		DWORD dwNumColorKeys) override;

	virtual HRESULT AddTextureHeightmapAsBump(UINT nSkinID, 
		const char *chName) override;

	virtual HRESULT ExchangeTexture(UINT nSkinID, 
		UINT nTexStage, 
		const char *chName, 
		bool bAlpha, 
		float fAlpha, 
		ZFXCOLOR *cColorKeys, 
		DWORD dwNumColorKeys) override;

	virtual HRESULT ExchangeMaterial(UINT nSkinID, 
		const ZFXCOLOR *pcAmb, 
		const ZFXCOLOR *pcDif, 
		const ZFXCOLOR *pcEmis, 
		const ZFXCOLOR *pcSpec, 
		float fSpecPower) override;

	virtual bool MaterialEqual(const ZFXMATERIAL *pMat0,
		const ZFXMATERIAL *pMat1) override;

	virtual bool ColorEqual(const ZFXCOLOR *pCol0, 
		const ZFXCOLOR *pCol1) override;

	virtual void LogCurrentStatus(char *chLog, 
		bool bDetailed) override;

	virtual UINT GetNumSkins(void) override;

	virtual ZFXSKIN GetSkin(UINT nSkinID) override;

	virtual ZFXMATERIAL GetMaterial(UINT nMatID) override;

	virtual const char* GetTextureName(UINT nTexID, 
		float *fAlpha, 
		ZFXCOLOR *AlphaKeys, 
		UCHAR *NumKeys) override;

	virtual ZFXTEXTURE GetTexture(UINT nTexID) override;

protected:
	void Log(char *fmt, ...);

	HRESULT CreateTexture(ZFXTEXTURE *pTexture, bool bAlpha);
	HRESULT ConvertToNormalmap(ZFXTEXTURE *pTexture);
	HRESULT SetAlphaKey(GLuint texture, UCHAR R, UCHAR G, UCHAR B, UCHAR A);
	HRESULT SetTransparency(GLuint texture, UCHAR alpha);

	

};


#endif