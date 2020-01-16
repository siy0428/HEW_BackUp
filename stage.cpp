#include <d3dx9.h>
#include "model.h"
#include "save_load.h"
#include "mydirectx.h"

static int modeleId;

void Stage_Init(void)
{
	modeleId = GetStageId();
	SetStage(modeleId);
}


void Stage_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	//FALSE:ライトOFF TRUE:ライトON

	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	Model_Draw(modeleId, mtxWorld);
}