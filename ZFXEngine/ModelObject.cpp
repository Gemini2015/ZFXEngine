#include "ZFXInclude.h"
#include "ModelObject.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <string.h>
#include <errno.h>

CModelObject::CModelObject(ZFXRenderDevice *pDevice)
{	
	m_pDevice = pDevice;
	m_file = "";
	m_bReady = false;
}


CModelObject::~CModelObject()
{
}

HRESULT CModelObject::LoadFromFile(std::string file)
{
	if (file.empty())
		return ZFX_INVALIDPARAM;

	m_file = file;
	m_bReady = false;
	FILE* fp;
	FILE* fp2;

	if ((fp = fopen(file.c_str(), "r")) == NULL)
		return ZFX_FAIL;


	std::vector<tagV> vlist;
	std::vector<tagVT> vtlist;
	std::vector<tagVN> vnlist;
	std::vector<tagRawSubModel> rawsubmodellist;

	char line[MAX_PATH];
	while (fgets(line, MAX_PATH, fp) != NULL)
	{
		char* content = strchr(line, ' ');
		if (content == NULL)
			continue;
		int tokenlen = content - line;
		char token[MAX_PATH];
		strncpy(token, line, tokenlen);
		token[tokenlen] = 0;
		content++;
		if (strcmp(token, "mtllib") == 0)
		{
			content[strlen(content) - 1] = 0;
			LoadMaterial(std::string(content));
		}
		else if (strcmp(token, "v") == 0)
		{
			tagV v;
			sscanf(content, "%f %f %f", &v.x, &v.y, &v.z);
			vlist.push_back(v);
		}
		else if (strcmp(token, "vt") == 0)
		{
			tagVT vt;
			sscanf(content, "%f %f", &vt.u, &vt.v);
			//vt.v = 1 - vt.v;
			vtlist.push_back(vt);
		}
		else if (strcmp(token, "vn") == 0)
		{
			tagVN vn;
			sscanf(content, "%f %f %f", &vn.x, &vn.y, &vn.z);
			vnlist.push_back(vn);
		}
		else if (strcmp(token, "usemtl") == 0)
		{
			tagRawSubModel rawsubmodel;
			rawsubmodel.materialID = m_SkinIDs[std::string(content)];
			LoadRawFace(rawsubmodel.flist, fp);

			rawsubmodellist.push_back(rawsubmodel);
		}
	}

	fclose(fp);

	VertexProcess(rawsubmodellist, vlist, vtlist, vnlist);

	CreateStaticBuffer();

	return ZFX_OK;
}

HRESULT CModelObject::Render()
{
	ZFXVertexCacheManager* vcm = m_pDevice->GetVertexManager();
	for (int i = 0; i < m_SubModel.size(); i++)
	{
		vcm->Render(m_SubModel[i].nSBID);
	}
	return ZFX_OK;
}

HRESULT CModelObject::LoadMaterial(std::string file)
{
	if (file.empty())
		return ZFX_INVALIDPARAM;

	m_materialfile = file;
	FILE* fp;
	if ((fp = fopen(file.c_str(), "r")) == NULL)
		return ZFX_FAIL;

	std::vector<tagRawMaterial> rawMatList;

	int current = -1;

	char line[MAX_PATH];
	while (fgets(line, MAX_PATH, fp) != NULL)
	{
		char* content = strchr(line, ' ');
		if(content == NULL)
			continue;
		int tokenlen = content - line;
		char token[MAX_PATH];
		strncpy(token, line, tokenlen);
		token[tokenlen] = 0;
		content++;
		if (strcmp(token, "newmtl") == 0)
		{
			tagRawMaterial mat;
			mat.name.assign(content);
			rawMatList.push_back(mat);
			current++;
		}
		else if (current != -1)
		{
			if (strcmp(token, "Ns") == 0)
			{
				sscanf(content, "%f", &rawMatList[current].ns);
			}
			else if (strcmp(token, "Ka") == 0)
			{
				float r, g, b;
				sscanf(content, "%f %f %f", &r, &g, &b);
				rawMatList[current].mat.cAmbient.fR = r;
				rawMatList[current].mat.cAmbient.fG = g;
				rawMatList[current].mat.cAmbient.fB = b;
				rawMatList[current].mat.cAmbient.fA = 1.0;
			}
			else if (strcmp(token, "Kd") == 0)
			{
				float r, g, b;
				sscanf(content, "%f %f %f", &r, &g, &b);
				rawMatList[current].mat.cDiffuse.fR = r;
				rawMatList[current].mat.cDiffuse.fG = g;
				rawMatList[current].mat.cDiffuse.fB = b;
				rawMatList[current].mat.cDiffuse.fA = 1.0;
			}
			else if (strcmp(token, "Ks") == 0)
			{
				float r, g, b;
				sscanf(content, "%f %f %f", &r, &g, &b);
				rawMatList[current].mat.cSpecular.fR = r;
				rawMatList[current].mat.cSpecular.fG = g;
				rawMatList[current].mat.cSpecular.fB = b;
				rawMatList[current].mat.cSpecular.fA = 1.0;
			}
			else if (strcmp(token, "Ni") == 0)
			{
				sscanf(content, "%f", &rawMatList[current].ni);
			}
			else if (strcmp(token, "d") == 0)
			{
				sscanf(content, "%f", &rawMatList[current].d);
			}
			else if (strcmp(token, "map_Kd") == 0)
			{
				content[strlen(content) - 1] = 0;
				rawMatList[current].texfile.push_back(std::string(content));
			}
			
		}
	}

	fclose(fp);

	ZFXSkinManager* sm = m_pDevice->GetSkinManager();

	for (int i = 0; i < rawMatList.size(); i++)
	{
		tagRawMaterial raw = rawMatList[i];
		UINT id;
		sm->AddSkin(&raw.mat.cAmbient, &raw.mat.cDiffuse, &raw.mat.cEmissive, &raw.mat.cSpecular, raw.mat.fPower, &id);
		for (int tex = 0; tex < raw.texfile.size(); tex++)
		{
			sm->AddTexture(id, raw.texfile[tex].c_str(), false, 1.0f, NULL, 0);
		}
		m_SkinIDs[raw.name] = id;
	}

	return ZFX_OK;
}

HRESULT CModelObject::LoadRawFace(std::vector<tagRawF> &rawface, FILE *fp)
{
	if (fp == NULL)
		return ZFX_INVALIDPARAM;

	char line[MAX_PATH];
	UINT uuid = -1;
	while (fgets(line, MAX_PATH, fp) != NULL)
	{
		char* content = strchr(line, ' ');
		int tokenlen = content - line;
		char token[MAX_PATH];
		strncpy(token, line, tokenlen);
		token[tokenlen] = 0;
		content++;

		if (strcmp(token, "f") == 0)
		{
			int v1, vt1, vn1;
			int v2, vt2, vn2;
			int v3, vt3, vn3;

			sscanf(content, "%d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3);

			tagRawF face;
			face.v1 = v1;
			face.v2 = v2;
			face.v3 = v3;
			face.vt1 = vt1;
			face.vt2 = vt2;
			face.vt3 = vt3;

			rawface.push_back(face);
		}
	}
	return ZFX_OK;
}

HRESULT CModelObject::VertexProcess(std::vector<tagRawSubModel> &rawsubmodellist,
	std::vector<tagV> vlist,
	std::vector<tagVT> vtlist,
	std::vector<tagVN> vnlist)
{
	std::vector<tagRawSubModel>::iterator it = rawsubmodellist.begin();
	WORD index = 0;
	while (it != rawsubmodellist.end())
	{
		// 遍历每一个原始子模型
		tagSubModel submodel;	
		submodel.nSkinID = it->materialID;
		std::vector<tagRawF>::iterator faceit = it->flist.begin();
		while (faceit != it->flist.end())
		{
			// 遍历每个子模型的面
			UINT64 uuid = 0;
			uuid |= (faceit->v1 & 0xffff);
			uuid |= (faceit->vt1 & 0xffff) << 16;
			// 查询是否存在该顶点
			INDEX_MAP::iterator indexit = m_IndexMap.find(uuid);
			if (indexit == m_IndexMap.end())
			{
				// 不存在该顶点组合
				VERTEX v;
				// v1
				v.x = vlist[faceit->v1 - 1].x;
				v.y = vlist[faceit->v1 - 1].y;
				v.z = vlist[faceit->v1 - 1].z;

				v.vcN[0] = v.vcN[1] = v.vcN[2] = 0;

				v.tu = vtlist[faceit->vt1 - 1].u;
				v.tv = vtlist[faceit->vt1 - 1].v;

				m_VertexList.push_back(v);
				index++;
				m_IndexMap[uuid] = index;
				submodel.Index.push_back(index);
			}
			else
			{
				submodel.Index.push_back(indexit->second);
			}

			// v2
			uuid |= (faceit->v2 & 0xffff);
			uuid |= (faceit->vt2 & 0xffff) << 16;
			// 查询是否存在该顶点
			indexit = m_IndexMap.find(uuid);
			if (indexit == m_IndexMap.end())
			{
				VERTEX v;
				v.x = vlist[faceit->v2 - 1].x;
				v.y = vlist[faceit->v2 - 1].y;
				v.z = vlist[faceit->v2 - 1].z;

				v.vcN[0] = v.vcN[1] = v.vcN[2] = 0;

				v.tu = vtlist[faceit->vt2 - 1].u;
				v.tv = vtlist[faceit->vt2 - 1].v;

				m_VertexList.push_back(v);
				index++;
				m_IndexMap[uuid] = index;
				submodel.Index.push_back(index);
			}
			else
			{
				submodel.Index.push_back(indexit->second);
			}
				
			// v3
			uuid |= (faceit->v3 & 0xffff);
			uuid |= (faceit->vt3 & 0xffff) << 16;
			// 查询是否存在该顶点
			indexit = m_IndexMap.find(uuid);
			if (indexit == m_IndexMap.end())
			{
				VERTEX v;
				v.x = vlist[faceit->v3 - 1].x;
				v.y = vlist[faceit->v3 - 1].y;
				v.z = vlist[faceit->v3 - 1].z;

				v.vcN[0] = v.vcN[1] = v.vcN[2] = 0;

				v.tu = vtlist[faceit->vt3 - 1].u;
				v.tv = vtlist[faceit->vt3 - 1].v;

				m_VertexList.push_back(v);
				index++;
				m_IndexMap[uuid] = index;
				submodel.Index.push_back(index);
			}
			else
			{
				submodel.Index.push_back(indexit->second);
			}
			faceit++;
		}

		m_SubModel.push_back(submodel);
		it++;
	}
	return ZFX_OK;
}

HRESULT CModelObject::CreateStaticBuffer()
{
	std::vector<tagSubModel>::iterator it = m_SubModel.begin();
	ZFXVertexCacheManager* vcm = m_pDevice->GetVertexManager();
	while (it != m_SubModel.end())
	{
		VERTEX* pVertex = new VERTEX[m_VertexList.size()];
		for (int i = 0; i < m_VertexList.size(); i++)
		{
			pVertex[i] = m_VertexList[i];
		}

		WORD* pIndex = new WORD[it->Index.size()];
		for (int i = 0; i < it->Index.size(); i++)
		{
			pIndex[i] = it->Index[i];
		}

		vcm->CreateStaticBuffer(VID_UU, it->nSkinID, m_VertexList.size(), it->Index.size(),
			pVertex, pIndex, &(it->nSBID));

		delete[] pVertex;
		delete[] pIndex;
		it++;
	}
	return ZFX_OK;
}
