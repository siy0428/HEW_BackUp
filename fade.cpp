#include "texture.h"
#include "sprite.h"
#include "fade.h"
#include "common.h"

//=====================================================
//�}�N����`
//=====================================================
#define FADE_INCRE (4)	//�␳�l

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static unsigned int g_Tex;
static int g_fade = 0;
static bool g_isUse = false;
static int g_incre = FADE_INCRE;
static SCENE g_scene = SCENE_NONE;
static bool g_half = false;

//=====================================================
//������
//=====================================================
void Fade_init(void)
{
	g_Tex = Texture_SetLoadFile("Texture\\black.jpeg", SCREEN_WIDTH, SCREEN_HEIGHT);
	g_fade = FADE_INCRE;
	g_isUse = false;
	g_incre = FADE_INCRE;
	g_scene = SCENE_NONE;
	g_half = false;
}

//=====================================================
//�I��
//=====================================================
void Fade_Uninit(void)
{

}

//=====================================================
//�X�V
//=====================================================
void Fade_Update(void)
{
	if (!g_isUse)
	{
		return;
	}

	//�����̖��x
	if (g_half)
	{
		g_fade += FADE_INCRE;
		if (g_fade > 128 - FADE_INCRE)
		{
			g_fade = 128 - FADE_INCRE;
		}
	}
	//�t�F�[�h�C���A�E�g
	else
	{
		g_fade += g_incre;
		//�t�F�[�h�C��
		if (g_fade > 255 - FADE_INCRE)
		{
			g_incre *= -1;
			Game_SceneChange(g_scene);
		}
		//�t�F�[�h�A�E�g
		else if (g_fade < FADE_INCRE)
		{
			g_incre *= -1;
			g_isUse = false;
		}
	}
}

//=====================================================
//�`��
//=====================================================
void Fade_Draw(void)
{
	if (g_isUse)
	{
		Sprite_Draw(g_Tex, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_fade);
	}
}

//=====================================================
//�g�p�����ǂ���
//=====================================================
bool Fade_isUse(void)
{
	return g_isUse;
}

//=====================================================
//�t�F�[�h����
//=====================================================
void Fade_InOut(SCENE scene)
{
	if (g_isUse)
	{
		return;
	}

	g_fade = FADE_INCRE;
	g_isUse = true;
	g_half = false;
	g_scene = scene;
}

//=====================================================
//�����̖��x�Ńt�F�[�h
//=====================================================
void Fade_Half(void)
{
	if (g_isUse)
	{
		return;
	}

	g_fade = FADE_INCRE;
	g_isUse = true;
	g_half = true;
}

//=====================================================
//�폜
//=====================================================
void Fade_Destroy(void)
{
	g_half = false;
}