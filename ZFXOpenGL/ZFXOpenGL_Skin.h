#ifndef _ZFXOPENGL_SKIN_H_
#define _ZFXOPENGL_SKIN_H_

#include "ZFXGLPrerequisites.h"
#include "ZFXRenderDevice.h"




class ZFXOpenGLSkinManager : public ZFXSkinManager
{
public:
	enum OpenGLSkinManager_Enum
	{
		MAX_ID = 65535,
	};

	ZFXOpenGLSkinManager(ZFXOpenGL *pOpenGL);
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
	ZFXOpenGL* m_pOpenGL;

	void Log(char *fmt, ...);

	HRESULT CreateTexture(ZFXTEXTURE *pTexture, bool bAlpha);
	HRESULT ConvertToNormalmap(ZFXTEXTURE *pTexture);
	HRESULT SetAlphaKey(GLuint texture, UCHAR R, UCHAR G, UCHAR B, UCHAR A);
	HRESULT SetTransparency(GLuint texture, UCHAR alpha);

	HRESULT ActiveSkin(UINT nSkinID);

};


#endif