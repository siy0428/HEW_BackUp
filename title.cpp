#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "input.h"
#include "game.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_tex;

//=====================================================
//������
//=====================================================
void Title_Init(void)
{
	g_tex = Texture_SetLoadFile("Texture\\title.png", 1280, 720);
}

//=====================================================
//�I��
//=====================================================
void Title_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Title_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Fade_InOut(SCENE_TUTORIAL);
	}
}

//=====================================================
//�`��
//=====================================================
void Title_Draw(void)
{
	Sprite_Draw(g_tex, 0, 0);
}