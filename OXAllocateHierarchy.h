#ifndef IKD_OX_OXALLOCATEHIERARVHY_H
#define IKD_OX_OXALLOCATEHIERARVHY_H

#include <d3dx9.h>

int ModelAnime_Init(const char *filename, const char *texname);
void ModelAnime_Uninit(void);
void ModelAnime_Draw(unsigned int index, float anime_speed, D3DXMATRIX mtxWorld, int anime_type);

namespace OX {
	struct OXD3DXFRAME : public D3DXFRAME {
		DWORD id;
		D3DXMATRIX offsetMatrix;
		OXD3DXFRAME() : id(0xffffffff) {
			D3DXMatrixIdentity(&offsetMatrix);
		}
	};

	struct OXD3DXMESHCONTAINER : public D3DXMESHCONTAINER {
		DWORD maxFaceInfl;
		DWORD numBoneCombinations;
		ID3DXBuffer *boneCombinationTable;
		OXD3DXMESHCONTAINER() : maxFaceInfl(1), numBoneCombinations(0), boneCombinationTable(0) {}
	};

	class AllocateHierarchy : public ID3DXAllocateHierarchy {
		char *copyName(const char* name) {
			char *n = 0;
			if (!name || name[0] == '\0') {
				n = new char[1];
				n[0] = '\0';
			}
			else {
				//size_t len = strlen(name);
				n = new char[strlen(name) + 1];
				strcpy_s(n, strlen(name) + 1, name);
			}
			return n;
		}

	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) {
			OXD3DXFRAME *newFrame = new OXD3DXFRAME;
			newFrame->Name = copyName(Name);
			newFrame->pFrameFirstChild = 0;
			newFrame->pFrameSibling = 0;
			newFrame->pMeshContainer = 0;
			D3DXMatrixIdentity(&newFrame->TransformationMatrix);
			*ppNewFrame = newFrame;

			return D3D_OK;
		}

		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) {
			if (pFrameToFree == NULL) {
				return D3D_OK;
			}
			if (pFrameToFree->pFrameFirstChild)
				DestroyFrame(pFrameToFree->pFrameFirstChild);
			if (pFrameToFree->pFrameSibling)
				DestroyFrame(pFrameToFree->pFrameSibling);
			if (pFrameToFree->pMeshContainer)
				DestroyMeshContainer(pFrameToFree->pMeshContainer);

			delete[] pFrameToFree->Name;
			delete pFrameToFree;

			return D3D_OK;
		}

		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA *pMeshData,
			CONST D3DXMATERIAL *pMaterials,
			CONST D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer
			) {
			OXD3DXMESHCONTAINER *newCont = new OXD3DXMESHCONTAINER;
			newCont->Name = copyName(Name);

			newCont->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
			memset(newCont->pAdjacency, 0, pMeshData->pMesh->GetNumFaces() * 3 * sizeof(DWORD));
			newCont->MeshData.Type = pMeshData->Type;
			pSkinInfo->ConvertToBlendedMesh(
				pMeshData->pMesh, 0, pAdjacency, newCont->pAdjacency, 0, 0, &newCont->maxFaceInfl,
				&newCont->numBoneCombinations, &newCont->boneCombinationTable, &newCont->MeshData.pMesh
			);

			newCont->NumMaterials = NumMaterials;
			newCont->pMaterials = new D3DXMATERIAL[NumMaterials];
			memcpy(newCont->pMaterials, pMaterials, NumMaterials * sizeof(D3DXMATERIAL));

			newCont->pEffects = 0;
			if (pEffectInstances) {
				newCont->pEffects = new D3DXEFFECTINSTANCE;
				newCont->pEffects->pEffectFilename = copyName(pEffectInstances->pEffectFilename);
				newCont->pEffects->NumDefaults = pEffectInstances->NumDefaults;
				newCont->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
				for (DWORD i = 0; i < pEffectInstances->NumDefaults; i++) {
					D3DXEFFECTDEFAULT *src = pEffectInstances->pDefaults + i;
					D3DXEFFECTDEFAULT *dest = newCont->pEffects->pDefaults + i;
					dest->NumBytes = src->NumBytes;
					dest->Type = src->Type;
					dest->pParamName = copyName(src->pParamName);
					dest->pValue = new char[src->NumBytes];
					memcpy(dest->pValue, src->pValue, src->NumBytes);
				}
			}

			newCont->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			*ppNewMeshContainer = newCont;

			return D3D_OK;
		}

		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) {
			OXD3DXMESHCONTAINER *m = (OXD3DXMESHCONTAINER*)pMeshContainerToFree;

			m->MeshData.pMesh->Release();
			delete[] m->Name;
			delete[] m->pAdjacency;
			if (m->pEffects) {
				for (DWORD i = 0; i < m->pEffects->NumDefaults; i++) {
					D3DXEFFECTDEFAULT *d = m->pEffects->pDefaults + i;
					delete[] d->pParamName;
					delete[] d->pValue;
				}
				delete[] m->pEffects->pDefaults;
				delete[] m->pEffects->pEffectFilename;
				delete m->pEffects;
			}
			delete[] m->pMaterials;

			if (m->pSkinInfo)
				m->pSkinInfo->Release();

			if (m->boneCombinationTable)
				m->boneCombinationTable->Release();

			delete m;

			return D3D_OK;
		}
	};
}

#endif