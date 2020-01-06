#include "model.h"
#include "cube.h"
#include "debug_font.h"
#include "input.h"
#include "OXAllocateHierarchy.h"
#include "mydirectx.h"

//====================================================
//�}�N����`
//====================================================
#define WHITE_BEAR_MAX (4)

//====================================================
//�񋓌^
//====================================================
typedef enum
{
	STOP,
	IDOLING,
	STEP
}ANIMETION_TYPE;

//====================================================
//�\���̐錾
//====================================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScale;
	ANIMETION_TYPE type;
	unsigned int modelId;
	bool isUse;
}WHITE_BEAR;

//====================================================
//�O���[�o���ϐ�
//====================================================
static WHITE_BEAR g_white_bear[WHITE_BEAR_MAX];

//====================================================
//������
//====================================================
void WhiteBear_Init(void)
{
	for (int i = 0; i < WHITE_BEAR_MAX; i++)
	{
		g_white_bear[i].type = IDOLING;
		g_white_bear[i].modelId = ModelAnime_Init("model\\sirokuma\\sirokuma.x", "model\\sirokuma\\sirokuma2.tga");
		g_white_bear[i].isUse = false;
		g_white_bear[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//g_white_bear[0].pos = D3DXVECTOR3(8.0f, 0.0f, 0.0f);
	//g_white_bear[1].pos = D3DXVECTOR3(-13.0f, 0.0f, 13.0f);
	//g_white_bear[2].pos = D3DXVECTOR3(8.0f, 0.0f, -8.0f);
	//g_white_bear[3].pos = D3DXVECTOR3(13.0f, 0.0f, 8.0f);
}

//====================================================
//�I��
//====================================================
void WhiteBear_Uninit(void)
{

}

//====================================================
//�X�V
//====================================================
void WhiteBear_Update(void)
{
	for (int i = 0; i < WHITE_BEAR_MAX; i++)
	{
		if (!g_white_bear[i].isUse)
		{
			continue;
		}
		D3DXMatrixTranslation(&g_white_bear[i].mtxWorld, g_white_bear[i].pos.x, g_white_bear[i].pos.y, g_white_bear[i].pos.z);
	}
}

//====================================================
//�`��
//====================================================
void WhiteBear_Draw(void)
{
	for (int i = 0; i < WHITE_BEAR_MAX; i++)
	{
		if (!g_white_bear[i].isUse)
		{
			continue;
		}
		ModelAnime_Draw(g_white_bear[i].modelId, 0.0001f, g_white_bear[i].mtxWorld, g_white_bear[i].type);
	}
}

//====================================================
//�X�e�[�W�G�f�B�^�̃f�[�^�擾
//====================================================
void WhiteBear_Set(D3DXMATRIX mtxWorld)
{
	for (int i = 0; i < WHITE_BEAR_MAX; i++)
	{
		if (g_white_bear[i].isUse)
		{
			continue;
		}
		g_white_bear[i].isUse = true;
		g_white_bear[i].mtxWorld = mtxWorld;
		g_white_bear[i].pos.x = mtxWorld._41;
		g_white_bear[i].pos.y = mtxWorld._42;
		g_white_bear[i].pos.z = mtxWorld._43;
		return;
	}
}

//====================================================
//�X�e�[�W�G�f�B�^�̃f�[�^�擾(�v�f���w��)
//====================================================
void WhiteBear_Set(D3DXMATRIX mtxWorld, int index)
{
	g_white_bear[index].isUse = true;
	g_white_bear[index].mtxWorld = mtxWorld;
	g_white_bear[index].pos.x = mtxWorld._41;
	g_white_bear[index].pos.y = mtxWorld._42;
	g_white_bear[index].pos.z = mtxWorld._43;
}