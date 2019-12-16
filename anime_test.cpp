#include "OXAllocateHierarchy.h"
#include "debug_font.h"
#include "model.h"
#include "mydirectx.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_anime_id[2];
static unsigned int g_test;

//=====================================================
//������
//=====================================================
void Anime_Test_Init(void)
{
	//g_anime_id[0] = ModelAnime_Init("model\\anime\\anime_test.x", "model\\anime\\");
	//g_anime_id[1] = ModelAnime_Init("model\\anime\\anime_test.x", "model\\anime\\");
}

//=====================================================
//�I��
//=====================================================
void Anime_Test_Uninit(void)
{
	//ModelAnime_Uninit();
}

//=====================================================
//�`��
//=====================================================
void Anime_Test_Draw(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DebugFont_Draw(0, 32 * 4, "%d", g_anime_id[0]);
	DebugFont_Draw(0, 32 * 5, "%d", g_anime_id[1]);
	static D3DXVECTOR3 pos;
	D3DXMATRIX mtxWorld;
	pos += D3DXVECTOR3(0.0f, 0.0f, 0.1f);
	//D3DXMatrixTranslation(&mtxWorld, pos.x, pos.y, pos.z);
	//ModelAnime_Draw(g_anime_id[0], 0.1f, mtxWorld);
	//D3DXMatrixTranslation(&mtxWorld, pos.x - 10.0f, pos.y, pos.z / 2);
	//ModelAnime_Draw(g_anime_id[1], 0.0001f, mtxWorld);
}