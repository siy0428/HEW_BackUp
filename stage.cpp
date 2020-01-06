#include <d3dx9.h>
#include "model.h"
#include "save_load.h"
#include "mydirectx.h"

void Stage_Init(void)
{
	
}


void Stage_Draw(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	//FALSE:ライトOFF TRUE:ライトON

	D3DXMATRIX mtxWorld;
	D3DXMatrixTranslation(&mtxWorld, 0.0f, -10.0f, 0.0f);
	Model_Draw(GetStageId(), mtxWorld);
}