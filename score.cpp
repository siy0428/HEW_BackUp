#include "number.h"
#include "texture.h"

static int g_Num[16] = {};
static int g_Score;
static int frameCount = 0;

void Score_Init(void)
{
	g_Score = 0;
}

void Score_Update(void)
{
	frameCount++;
}

void Score_Draw(int score, float x, float y, int digit, bool left)
{
	score += g_Score;
	for (int i = 0; i < digit; i++)
	{
		g_Num[i] = score % 10;
		if (!left)
		{
			Number_Draw(g_Num[i], x - i * 20, y);
		}
		else
		{
			Number_Draw(g_Num[i], x + i * 20, y);
		}
		score /= 10;
	}
}

void Score_Add(int score)
{
	g_Score += score;
}