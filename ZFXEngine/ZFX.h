#ifndef _ZFX_H_
#define _ZFX_H_

#include <windows.h>

#include "zfx3d.h"
#include "..\ZFXUtil\ZFXUtil.h"


// everything went smooth
#define ZFX_OK                        S_OK

// just reports no errors
#define ZFX_CANCELED                  0x02000001

// generell error message
#define ZFX_FAIL                      0x82000001

// specific error messages
#define ZFX_CREATEAPI                 0x82000002
#define ZFX_CREATEDEVICE              0x82000003
#define ZFX_CREATEBUFFER              0x82000004
#define ZFX_INVALIDPARAM              0x82000005
#define ZFX_INVALIDID                 0x82000006
#define ZFX_BUFFERSIZE                0x82000007
#define ZFX_BUFFERLOCK                0x82000008
#define ZFX_NOTCOMPATIBLE             0x82000009
#define ZFX_OUTOFMEMORY               0x8200000a
#define ZFX_FILENOTFOUND              0x8200000b
#define ZFX_INVALIDFILE               0x8200000c
#define ZFX_NOSHADERSUPPORT           0x8200000d
#define ZFX_FAILa               0x8200000c
#define ZFX_FAILb               0x8200000d
#define ZFX_FAILc               0x8200000e
#define ZFX_FAILd               0x8200000f

#ifdef _DEBUG

#define DEBUG_LOG(str) { extern ZFXLogger* GetLogger(); GetLogger()->Print();}

#else

#define DEBUG_LOG(str) 

#endif

// VERTEX TYPES:

typedef struct PVERTEX_TYPE {
	float	 x, y, z;
} PVERTEX;

typedef struct VERTEX_TYPE {
	float	 x, y, z;
	float  vcN[3];
	float  tu, tv;
} VERTEX;

typedef struct LVERTEX_TYPE {
	float	 x, y, z;
	union 
	{
		struct
		{
			BYTE r;
			BYTE g;
			BYTE b;
			BYTE a;
		};
		DWORD  Color;
	};
	
	float  tu, tv;
} LVERTEX;

typedef struct CVERTEX_TYPE {
	float	 x, y, z;
	float  vcN[3];
	float  tu, tv;
	float  fBone1, fWeight1;
	float  fBone2, fWeight2;
} CVERTEX;

typedef struct VERTEX3T_TYPE {
	float	 x, y, z;
	float  vcN[3];
	float  tu0, tv0;
	float  tu1, tv1;
	float  tu2, tv2;
} VERTEX3T;

typedef struct TVERTEX_TYPE {
	float	 x, y, z;
	float  vcN[3];
	float  tu, tv;
	float  vcU[3];
} TVERTEX;

/*----------------------------------------------------------------*/


// ENUMERATED TYPES:

typedef enum ZFXVERTEXID_TYPE {
	VID_NONE = -1,
	VID_PS,       // untransformed position only
	VID_UU,       // untransformed and unlit vertex
	VID_UL,       // untransformed and lit vertex vertex
	VID_CA,       // used for character animation
	VID_3T,       // three texture coord pairs
	VID_TV        // like UU but with tangent vector
} ZFXVERTEXID;

typedef enum VERTEX_ATTRIB_TYPE
{
	VAT_POSITION = 0,
	VAT_COLOR = 1,
	VAT_NORMAL = 2,
	VAT_TEXTURE_COORD1 = 3,
	VAT_TEXTURE_COORD2 = 4, 
	VAT_TEXTURE_COORD3 = 5,
};

typedef enum ZFXENGINEMODE_TYPE {
	EMD_PERSPECTIVE,      // perspective projection
	EMD_TWOD,             // world coords equal to screen coords
	EMD_ORTHOGONAL        // orthogonal projection
} ZFXENGINEMODE;

typedef enum ZFXLIGHTID_TYPE {
	LGT_DIRECTIONAL,  // directional light source
	LGT_POINT,        // point light source
	LGT_SPOT          // spot light source
} ZFXLIGHTID;

typedef enum ZFXDATATYPE_TYPE {
	DAT_BOOL,          // boolean
	DAT_INT,           // integer
	DAT_FLOAT          // floating point
} ZFXDATATYPE;

typedef enum ZFXSHADER_TYPE {
	SHT_NONE,
	SHT_VERTEX,
	SHT_PIXEL
} ZFXSHADERTYPE;

typedef enum ZFXRENDERSTATE_TYPE {
	RS_NONE,             // just nothing
	
	RS_CULL_CW,          // cull clockwise ordered triangles
	RS_CULL_CCW,         // cull counter cw ordered triangles
	RS_CULL_NONE,        // render front- and backsides
	
	RS_DEPTH_READWRITE,  // read and write depth buffer
	RS_DEPTH_READONLY,   // read but don't write depth buffer
	RS_DEPTH_NONE,       // no read or write with depth buffer
	
	RS_SHADE_POINTS,     // render just vertices
	RS_SHADE_LINES,      // render two verts as one line
	RS_SHADE_TRIWIRE,    // render triangulated wire
	RS_SHADE_HULLWIRE,   // render poly hull as polyline
	RS_SHADE_SOLID,      // render solid polygons
	RS_SHADE_QUADS,		 // render quad
	
	RS_TEX_ADDSIGNED,    // texture stage operation
	RS_TEX_MODULATE,     // texture stage operation
	
	RS_STENCIL_DISABLE,        // stencilbuffer off
	RS_STENCIL_ENABLE,         // stencilbuffer off
	RS_STENCIL_FUNC_ALWAYS,    // stencil pass mode
	RS_STENCIL_FUNC_LESSEQUAL, // stencil pass mode
	RS_STENCIL_MASK,           // stencil mask
	RS_STENCIL_WRITEMASK,      // stencil write mask
	RS_STENCIL_REF,            // reference value
	RS_STENCIL_FAIL_DECR,      // stencil fail decrements
	RS_STENCIL_FAIL_INCR,      // stencil fail increments
	RS_STENCIL_FAIL_KEEP,      // stencil fail keeps
	RS_STENCIL_ZFAIL_DECR,     // stencil pass but z fail decrements
	RS_STENCIL_ZFAIL_INCR,     // stencil pass but z fail increments
	RS_STENCIL_ZFAIL_KEEP,     // stencil pass but z fail keeps
	RS_STENCIL_PASS_DECR,      // stencil and z pass decrements
	RS_STENCIL_PASS_INCR,      // stencil and z pass increments
	RS_STENCIL_PASS_KEEP,      // stencil and z pass keeps
	RS_DEPTHBIAS               // bis value to add to depth value
} ZFXRENDERSTATE;
/*----------------------------------------------------------------*/

// STRUCTURES:
// plain viewport type
typedef struct ZFXVIEWPORT_TYPE {
	DWORD X;    // position of upper
	DWORD Y;    // ... left corner
	DWORD Width;
	DWORD Height;
} ZFXVIEWPORT;

struct _ZFXCOLOR
{
	union
	{
		struct
		{
			float fR;
			float fG;
			float fB;
			float fA;
		};
		float c[4];
	};
	_ZFXCOLOR()
	{
		memset(c, 0, sizeof(c));
	}
	bool operator==(const _ZFXCOLOR& clr1) const
	{
		return memcmp(&this->c[0], &clr1.c[0], sizeof(_ZFXCOLOR)) == 0 ? true : false;
	}
};
typedef _ZFXCOLOR ZFXCOLOR;

typedef struct _ZFXMaterial
{
	ZFXCOLOR cDiffuse;
	ZFXCOLOR cAmbient;
	ZFXCOLOR cSpecular;
	ZFXCOLOR cEmissive;
	float fPower;
	_ZFXMaterial()
	{
		fPower = 0;
	}
	bool operator==(const _ZFXMaterial& m) const
	{
		if (this->cDiffuse == m.cDiffuse &&
			this->cAmbient == m.cAmbient &&
			this->cSpecular == m.cSpecular &&
			this->cEmissive == m.cEmissive &&
			this->fPower == m.fPower)
			return true;
		return false;
	}
}ZFXMATERIAL;

typedef struct _ZFXTexture
{
	float fAlpha;
	char *chName;
	void *pData;
	ZFXCOLOR *pClrKeys;
	DWORD dwNum;
	_ZFXTexture()
	{
		fAlpha = 0;
		chName = NULL;
		pData = NULL;
		pClrKeys = NULL;
		dwNum = 0;
	}
}ZFXTEXTURE;

typedef struct _ZFXSkin
{
	bool bAlpha;
	UINT nMaterial;
	UINT nTexture[8];
	_ZFXSkin()
	{
		bAlpha = false;
		nMaterial = 0;
		memset(nTexture, 0, sizeof(nTexture));
	}
}ZFXSKIN;

// light type
typedef struct ZFXLIGHT_TYPE {
	ZFXLIGHTID Type;           // type of light
	ZFXCOLOR   cDiffuse;       // RGBA diffuse light value
	ZFXCOLOR   cSpecular;      // RGBA specular light value
	ZFXCOLOR   cAmbient;       // RGBA ambient light value
	ZFXVector  vcPosition;     // light position
	ZFXVector  vcDirection;    // light direction
	float      fRange;         // range of light
	float      fTheta;         // angle of spot light inner cone
	float      fPhi;           // angle of spot light outer cone
	float      fAttenuation0;  // change of intensity over distance
	float      fAttenuation1;  // change of intensity over distance
} ZFXLIGHT;
/*----------------------------------------------------------------*/





#endif