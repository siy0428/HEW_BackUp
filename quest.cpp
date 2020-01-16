//��{
#include "debug_font.h"
#include "input.h"

//�����p
#include <windows.h>
#include <time.h>

//�`��
#include "texture.h"
#include "sprite.h"

//====================================================
//�}�N����`
//====================================================


//====================================================
//�񋓌^
//====================================================
typedef enum
{
	PLAYER_1_THROW,			//1P�̃X�g�[���ɂԂ���
	PLAYER_2_THROW,			//2P�̃X�g�[���ɂԂ���
	PLAYER_3_THROW,			//3P�̃X�g�[���ɂԂ���
	PLAYER_4_THROW,			//4P�̃X�g�[���ɂԂ���
	STONE_THROW,			//�w�肵���X�g�[���œ�����
	RANGE_THROW,			//�X�g�[�������̋���������
	CURVE_THROW,			//�J�[�u�������ē�����
	STONE_HIT_POND,			//���̃X�g�[����e���Ēr�ɗ��Ƃ�
	STONE_HIT_BEAR,			//���̃X�g�[����e���Ĕ��F�ɂԂ���
	STONE_HIT_PENGUIN,		//���̃X�g�[����e���ăy���M���ɂԂ���
	FALL_ICICLES,			//���𗎂Ƃ�
	QUEST_MAX				//�ő�N�G�X�g��
}QUEST_TYPE;

//====================================================
//�O���[�o���ϐ�
//====================================================
static int g_tex[QUEST_MAX];
static QUEST_TYPE g_type;

//====================================================
//������
//====================================================
void Quest_Init(void)
{
	//�b���ŗ����̃V�[�h�l�擾
	srand((unsigned int)time(NULL));
	//�e�N�G�X�g�̃e�N�X�`���擾
	g_tex[PLAYER_1_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_2_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_3_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[PLAYER_4_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[RANGE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[CURVE_THROW] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_POND] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_BEAR] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[STONE_HIT_PENGUIN] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_tex[FALL_ICICLES] = Texture_SetLoadFile("Texture\\", 0, 0);
	g_type = (QUEST_TYPE)1;
}

//====================================================
//�I��
//====================================================
void Quest_Uninit(void)
{

}

//====================================================
//�X�V
//====================================================
void Quest_Update(void)
{
	int randum = rand() % QUEST_MAX;
	if (Keyboard_IsTrigger(DIK_Q))
	{
		g_type = (QUEST_TYPE)((int)g_type + 1);
	}
}

//====================================================
//�`��
//====================================================
void Quest_Draw(D3DXVECTOR3 pos)
{
	DebugFont_Draw(0, 32 * 5, "type = %d", g_type);
	Sprite_Draw(g_tex[g_type], pos.x, pos.y);
}

//====================================================
//����
//====================================================
void Quest_Create(void)
{
	int randum = rand() % QUEST_MAX;
	g_type = (QUEST_TYPE)randum;
}

//�^�C�v���ƂɃN�G�X�g�`��
//switch (g_type)
//{
//case PLAYER_1_THROW:
//	break;
//case PLAYER_2_THROW:
//	break;
//case PLAYER_3_THROW:
//	break;
//case PLAYER_4_THROW:
//	break;
//case STONE_THROW:
//	break;
//case RANGE_THROW:
//	break;
//case CURVE_THROW:
//	break;
//case STONE_HIT_POND:
//	break;
//case STONE_HIT_BEAR:
//	break;
//case STONE_HIT_PENGUIN:
//	break;
//case FALL_ICICLES:
//	break;
//}