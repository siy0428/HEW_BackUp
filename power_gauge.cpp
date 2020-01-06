#include "sprite.h"
#include "texture.h"
#include "mouse.h"
#include "common.h"

//=====================================================
//�}�N����`
//=====================================================
#define FACTOR (80)	//�Q�[�W�␳�p���q

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_tex[4] = {};
static int g_gauge;

//=====================================================
//������
//=====================================================
void Pow_Gauge_Init(void)
{
	g_tex[0] = Texture_SetLoadFile("Texture\\gauge.png", 128 * SCREEN_WIDTH / 1280, (512 - FACTOR * 2) * SCREEN_HEIGHT / 720);		//�Q�[�W
	g_tex[1] = Texture_SetLoadFile("Texture\\gauge_frame.png", 128 * SCREEN_WIDTH / 1280, 512 * SCREEN_HEIGHT / 720);		//�Q�[�W�g
	g_gauge = 0;
}

//=====================================================
//�I��
//=====================================================
void Pow_Gauge_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Pow_Gauge_Update(void)
{
	//�}�E�X�̒l�擾
	g_gauge = Mouse_GetForce();
}

//=====================================================
//�`��
//=====================================================
void Pow_Gauge_Draw(void)
{
	//�Q�[�W�̕�����
	int width = Texture_GetWidth(g_tex[0]);
	int height = Texture_GetHeight(g_tex[0]);

	//�Q�[�W�g�̕�����
	int width2 = Texture_GetWidth(g_tex[1]);
	int height2 = Texture_GetHeight(g_tex[1]);

	//�Q�[�W�`��
	Sprite_Draw(width, height, SCREEN_WIDTH / 25, SCREEN_HEIGHT / 6, 0, 0, width2, height2, D3DCOLOR_RGBA(0, 0, 0, 255));
	Sprite_Draw(g_tex[0], (float)SCREEN_WIDTH / 25, (float)(SCREEN_HEIGHT / 6 + height) - height * g_gauge / 100 + FACTOR, (float)width, (float)height * g_gauge / 100, 0, 0, width, height);
	Sprite_Draw(g_tex[1], (float)SCREEN_WIDTH / 25, (float)SCREEN_HEIGHT / 6);
}