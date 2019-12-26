#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>
#include "OXAllocateHierarchy.h"
#include "mydirectx.h"
#include "texture.h"
#include "input.h"

//=====================================================
//マクロ定義
//=====================================================
#define ANIME_MAX (32)

//=====================================================
//構造体宣言
//=====================================================
typedef struct
{
	char filename[256];
	std::map<DWORD, D3DXMATRIX> combMatrixMap;
	OX::AllocateHierarchy allocater;
	OX::OXD3DXFRAME *pRootFrame = 0;
	LPD3DXANIMATIONCONTROLLER controller;
	unsigned int tex;
	D3DXBONECOMBINATION *combs;
	OX::OXD3DXMESHCONTAINER *cont;
	LPD3DXANIMATIONSET pAnimeSet[16];
}ANIME;

//=====================================================
//グローバル変数
//=====================================================

//static 	std::map<DWORD, D3DXMATRIX> g_combMatrixMap;
//static OX::AllocateHierarchy g_allocater;
//static OX::OXD3DXFRAME *g_pRootFrame = 0;
//static ID3DXAnimationController *g_controller = 0;
//static unsigned int g_tex;
//D3DXBONECOMBINATION *g_combs;
//OX::OXD3DXMESHCONTAINER *g_cont;
ANIME g_anime[ANIME_MAX];

OX::OXD3DXMESHCONTAINER *getMeshContainer(D3DXFRAME *frame) {
	if (frame->pMeshContainer)
		return (OX::OXD3DXMESHCONTAINER*)frame->pMeshContainer;
	if (frame->pFrameFirstChild) {
		OX::OXD3DXMESHCONTAINER *mesh = getMeshContainer(frame->pFrameFirstChild);
		if (mesh != 0)
			return mesh;
	}
	if (frame->pFrameSibling)
		return getMeshContainer(frame->pFrameSibling);
	return 0;
}

void setFrameId(OX::OXD3DXFRAME *frame, ID3DXSkinInfo *info) {
	std::map<std::string, DWORD> nameToIdMap;
	for (DWORD i = 0; i < info->GetNumBones(); i++)
		nameToIdMap[info->GetBoneName(i)] = i;

	struct create {
		static void f(std::map<std::string, DWORD> nameToIdMap, ID3DXSkinInfo *info, OX::OXD3DXFRAME* frame) {
			if (nameToIdMap.find(frame->Name) != nameToIdMap.end()) {
				frame->id = nameToIdMap[frame->Name];
				frame->offsetMatrix = *info->GetBoneOffsetMatrix(frame->id);
			}
			if (frame->pFrameFirstChild)
				f(nameToIdMap, info, (OX::OXD3DXFRAME*)frame->pFrameFirstChild);
			if (frame->pFrameSibling)
				f(nameToIdMap, info, (OX::OXD3DXFRAME*)frame->pFrameSibling);
		}
	};
	create::f(nameToIdMap, info, frame);
}

void updateCombMatrix(std::map<DWORD, D3DXMATRIX> &combMatrixMap, OX::OXD3DXFRAME *frame) {
	struct update {
		static void f(std::map<DWORD, D3DXMATRIX> &combMatrixMap, D3DXMATRIX &parentBoneMatrix, OX::OXD3DXFRAME *frame) {
			D3DXMATRIX &localBoneMatrix = frame->TransformationMatrix;
			D3DXMATRIX boneMatrix = localBoneMatrix * parentBoneMatrix;
			if (frame->id != 0xffffffff)
				combMatrixMap[frame->id] = frame->offsetMatrix * boneMatrix;
			if (frame->pFrameFirstChild)
				f(combMatrixMap, boneMatrix, (OX::OXD3DXFRAME*)frame->pFrameFirstChild);
			if (frame->pFrameSibling)
				f(combMatrixMap, parentBoneMatrix, (OX::OXD3DXFRAME*)frame->pFrameSibling);
		}
	};
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	update::f(combMatrixMap, iden, frame);
}

//=====================================================
//初期化
//=====================================================
unsigned int ModelAnime_Init(const char *filename, const char *texname)
{
	////すでにあった場合
	//for (int i = 0; i < ANIME_MAX; i++)
	//{
	//	if (strcmp(g_anime[i].filename, filename) == 0)
	//	{
	//		return i;
	//	}
	//}
	//新規登録
	for (int i = 0; i < ANIME_MAX; i++)
	{
		if (g_anime[i].filename[0] != '\0')
		{
			continue;
		}
		strcpy(g_anime[i].filename, filename);
		//デバイスのポインタ取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// スキンメッシュ情報をXファイルから取得
		D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, pDevice, &g_anime[i].allocater, 0, (D3DXFRAME**)&g_anime[i].pRootFrame, &g_anime[i].controller);

		//アニメーションの数分読み込み
		for (UINT j = 0; j < g_anime[i].controller->GetNumAnimationSets(); j++)
		{
			g_anime[i].controller->GetAnimationSet(j, &g_anime[i].pAnimeSet[j]);
		}

		g_anime[i].cont = getMeshContainer(g_anime[i].pRootFrame);
		g_anime[i].combs = (D3DXBONECOMBINATION*)g_anime[i].cont->boneCombinationTable->GetBufferPointer();

		// フレーム内にボーンIDとオフセット行列を埋め込む
		setFrameId(g_anime[i].pRootFrame, g_anime[i].cont->pSkinInfo);
		//テクスチャ生成
		g_anime[i].tex = Texture_SetLoadFile(texname, 0, 0);

		return i;
	}
	return -1;
}

//=====================================================
//終了
//=====================================================
void ModelAnime_Uninit(void)
{
	for (int i = 0; i < ANIME_MAX; i++)
	{
		if (g_anime[i].filename[0] != '\0')
		{
			continue;
		}
		// スキンメッシュ情報削除
		g_anime[i].allocater.DestroyFrame(g_anime[i].pRootFrame);
	}
}
static int button = 0;
//=====================================================
//描画
//=====================================================
void ModelAnime_Draw(unsigned int index, float anime_speed, D3DXMATRIX mtxWorld)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		button++;
		button %= 3;
	}

	g_anime[index].controller->SetTrackAnimationSet(0, g_anime[index].pAnimeSet[button]);
	g_anime[index].controller->SetTrackSpeed(0, 1);
	g_anime[index].controller->SetTrackWeight(0, 0.1f);
	g_anime[index].controller->SetTrackWeight(1, 1.0f);

	g_anime[index].controller->AdvanceTime(anime_speed, 0);

	updateCombMatrix(g_anime[index].combMatrixMap, g_anime[index].pRootFrame);

	pDevice->SetTexture(0, Texture_GetTexture(g_anime[index].tex));

	for (DWORD i = 0; i < g_anime[index].cont->numBoneCombinations; i++) {
		DWORD boneNum = 0;
		for (DWORD j = 0; j < g_anime[index].cont->maxFaceInfl; j++) {
			DWORD id = g_anime[index].combs[i].BoneId[j];
			if (id != UINT_MAX) {
				pDevice->SetTransform(D3DTS_WORLDMATRIX(j), &(g_anime[index].combMatrixMap[id] * mtxWorld));
				boneNum++;
			}
		}
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, boneNum - 1);
		g_anime[index].cont->MeshData.pMesh->DrawSubset(i);
	}
}