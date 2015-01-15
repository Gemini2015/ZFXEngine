#pragma once

#include "ZFXPrerequisites.h"

#include <vector>
#include <map>

class CModelObject
{
	typedef struct tagV
	{
		float x, y, z;
	}tagV;

	typedef struct tagVT
	{
		float u, v;
	}tagVT;

	typedef struct tagVN
	{
		float x, y, z;
	}tagVN;

	typedef struct tagRawF
	{
		UINT v1, v2, v3;
		UINT vt1, vt2, vt3;
		UINT vn1, vn2, vn3;
	}tagRawF;

	typedef struct tagRawSubModel
	{
		UINT materialID;
		std::vector<tagRawF> flist;
	}tagRawSubModel;

	typedef struct tagRawMaterial
	{
		std::string name;
		float ns;
		float ni;
		float d;
		ZFXMATERIAL mat;

		std::vector<std::string> texfile;
	}tagRawMaterial;

	// 所有顶点
	std::vector<VERTEX> m_VertexList;

	// 一个SubModel代表使用同一Skin的所有顶点索引
	typedef struct tagSubModel
	{
		UINT nSkinID;
		std::vector<WORD> Index;
		UINT nSBID;
	}tagSubModel;

	std::vector<tagSubModel> m_SubModel;
	// 所有Skin ID
	std::map<std::string, UINT> m_SkinIDs;

	std::string m_file;
	std::string m_materialfile;
	bool m_bReady;

	ZFXRenderDevice *m_pDevice;

	typedef std::map<UINT64, WORD> INDEX_MAP;
	std::map<UINT64, WORD> m_IndexMap;

public:
	enum VERTEX_ATTRIB
	{
		VA_VERTEX = 1,
		VA_NORMAL = 2,
		VA_TEX_COORD0 = 4,
	};

	DWORD m_VertexAttribFlag;

	CModelObject(ZFXRenderDevice *pDevice);
	~CModelObject();

	HRESULT LoadFromFile(std::string file);

	

	HRESULT Render();

	DWORD GetVertexAttribFlag();

private:
	HRESULT LoadMaterial(std::string file);
	HRESULT LoadRawFace(std::vector<tagRawF> &rawface, FILE *fp);
	HRESULT VertexProcess(std::vector<tagRawSubModel> &rawsubmodellist,
		std::vector<tagV> vlist,
		std::vector<tagVT> vtlist,
		std::vector<tagVN> vnlist);

	HRESULT CreateStaticBuffer();
};

