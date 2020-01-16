#include "number.h"
#include "texture.h"

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static int g_Num[16] = {};
static int g_Score;
static int frameCount = 0;

//=====================================================
//������
//=====================================================
void Score_Init(void)
{
	g_Score = 0;
	Number_Init();
}

//=====================================================
//�X�V
//=====================================================
void Score_Update(void)
{
	frameCount++;
}

//=====================================================
//�`��
//=====================================================
void Score_Draw(int score, float x, float y, int digit, bool left)
{
	//�X�R�A���}�C�i�X���������x�v���X��
	if (score < 0)
	{
		score *= -1;
		Number_Draw(x - 60 * digit, y);
	}
	score += g_Score;
	for (int i = 0; i < digit; i++)
	{
		g_Num[i] = score % 10;
		if (!left)
		{
			Number_Draw(g_Num[i], x - i * 60, y);
		}
		else
		{
			Number_Draw(g_Num[i], x + i * 60, y);
		}
		score /= 10;
	}
}

//=====================================================
//�T�C�Y�����`��
//=====================================================
void Score_Draw(int score, float size, float x, float y, int digit, bool left = false)
{
	//�X�R�A���}�C�i�X���������x�v���X��
	if (score < 0)
	{
		score *= -1;
		Number_Draw(x - 150 * digit, y);
	}
	score += g_Score;
	for (int i = 0; i < digit; i++)
	{
		g_Num[i] = score % 10;
		if (!left)
		{
			Number_Draw(g_Num[i], x - i * 150, y, size);
		}
		else
		{
			Number_Draw(g_Num[i], x + i * 150, y, size);
		}
		score /= 10;
	}
}

//=====================================================
//�X�R�A���Z
//=====================================================
void Score_Add(int score)
{
	g_Score += score;
}