#include "model.h"
#include "cube.h"
#include "debug_font.h"
#include "input.h"

//====================================================
//�}�N����`
//====================================================
#define PENGUIN_MAX (4)

//====================================================
//�񋓌^
//====================================================
//�E�ƍ�
typedef enum
{
	LEFT,
	RIGHT,
	LEG_MAX
}LEG_LR;

//�y���M���̃A�j���[�V�������
typedef enum
{
	IDOLING,
	STEP
}ANIMETION_TYPE;

//====================================================
//�\���̐錾
//====================================================

//�y���M���̑�
typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxMove;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScale;
	D3DXMATRIX mtxTransBody;
	D3DXMATRIX mtxSin;
	D3DXVECTOR3 pos;
}PENGUIN_LEG;

//�y���M���̑�
typedef struct
{
	D3DXMATRIX  mtxWorld;
	D3DXMATRIX  mtxTrans;
	D3DXMATRIX  mtxMove;
	D3DXMATRIX  mtxRot;
	D3DXMATRIX  mtxRotCenter;
	D3DXMATRIX  mtxScale;
	D3DXVECTOR3 pos;
}PENGUIN_BODY;

//�y���M��
typedef struct
{
	PENGUIN_LEG penguin_leg[LEG_MAX];
	PENGUIN_BODY penguin_body;
	D3DXMATRIX rot;
	bool animation = false;
	float move;
	float rot_speed;
	float rot_center;
	float anim_move;
	ANIMETION_TYPE type;
	LEG_LR leg_index;
}PENGUIN;

//====================================================
//�O���[�o���ϐ�
//====================================================
static unsigned int g_modelId = NULL;
static PENGUIN *g_penguin;
static int g_frame = 0;

//====================================================
//������
//====================================================
void Penguin_Init(void)
{
	//���I�������m��
	g_penguin = new PENGUIN[PENGUIN_MAX];
	//�̏�����
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		g_penguin[i].penguin_body.pos = D3DXVECTOR3(0.0f + i * -5.0f, 2.0f, 0.0f);
		g_penguin[i].move = 0.0f;
		g_penguin[i].rot_speed = 0.0f;
		g_penguin[i].animation = false;
		g_penguin[i].anim_move = 0.0f;
		g_penguin[i].rot_center = 0.5f;
		g_penguin[i].leg_index = LEFT;
		//�����̏�����
		for (int j = 0; j < LEG_MAX; j++)
		{
			g_penguin[i].penguin_leg[j].pos = D3DXVECTOR3(-1.0f + j * 2.0f, 1.0f, 0.0f);
		}
	}
	g_frame = 0;
}

//====================================================
//�I��
//====================================================
void Penguin_Uninit(void)
{
	delete[] g_penguin;
}

//====================================================
//�X�V
//====================================================
void Penguin_Update(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		//�e�X�g�p�L�[����
		if (Keyboard_IsPress(DIK_U))
		{
			g_penguin[i].move = 0.05f;
		}
		else if (Keyboard_IsPress(DIK_J))
		{
			g_penguin[i].move = -0.05f;
		}
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxMove, 0.0f, 0.0f, g_penguin[i].move);
		g_penguin[i].move *= 0.8f;
		//����]
		if (Keyboard_IsPress(DIK_H))
		{
			g_penguin[i].rot_speed += -0.05f;
		}
		//�E��]
		else if (Keyboard_IsPress(DIK_K))
		{
			g_penguin[i].rot_speed += 0.05f;
		}
		D3DXMatrixRotationY(&g_penguin[i].rot, g_penguin[i].rot_speed);

		//��
		//D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxWorld, g_penguin[i].penguin_body.pos.x - g_penguin[i].rot_center, g_penguin[i].penguin_body.pos.y, g_penguin[i].penguin_body.pos.z);
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxTrans, g_penguin[i].penguin_body.pos.x, g_penguin[i].penguin_body.pos.y, g_penguin[i].penguin_body.pos.z);
		
		//����
		for (int j = 0; j < LEG_MAX; j++)
		{
			D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxWorld, g_penguin[i].penguin_leg[j].pos.x, g_penguin[i].penguin_leg[j].pos.y, g_penguin[i].penguin_leg[j].pos.z);
			if (j == LEFT)
			{
				D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxTransBody, -1.0f, 0.0f, 0.0f);
			}
			else if (j = RIGHT)
			{
				D3DXMatrixTranslation(&g_penguin[i].penguin_leg[j].mtxTransBody, 1.0f, 0.0f, 0.0f);
			}
		}
		//�̂̉�]
		D3DXMatrixRotationZ(&g_penguin[i].penguin_body.mtxRot, (FLOAT)sin(g_penguin[i].anim_move) / 8);

		//sin��0�t�߂ɂȂ����璆�S�؂�ւ�
		//�����グ
		if (sin(g_penguin[0].anim_move) / 8 >= 0.0f)
		{
			g_penguin[i].rot_center = 0.5f;
			g_penguin[i].leg_index = RIGHT;
		}
		//�E���グ
		else
		{
			g_penguin[i].rot_center = -0.5f;
			g_penguin[i].leg_index = LEFT;
		}
		//���S�ړ�
		D3DXMatrixTranslation(&g_penguin[i].penguin_body.mtxRotCenter, g_penguin[i].rot_center, 0.0f, 0.0f);

		//���̓���
		D3DXMatrixTranslation(&g_penguin[i].penguin_leg[LEFT].mtxSin, 0.0f, 0.0f, (float)cos(g_penguin[i].anim_move) / 2);
		D3DXMatrixTranslation(&g_penguin[i].penguin_leg[RIGHT].mtxSin, 0.0f, 0.0f, -(float)cos(g_penguin[i].anim_move) / 2);

		g_penguin[i].penguin_leg[LEFT].pos.z = g_penguin[i].penguin_body.pos.z;
		g_penguin[i].penguin_leg[RIGHT].pos.z = g_penguin[i].penguin_body.pos.z;
		g_penguin[i].penguin_leg[LEFT].pos.y = g_penguin[i].penguin_body.pos.y - 1.0f - (float)sin(g_penguin[i].anim_move) / 4;
		g_penguin[i].penguin_leg[RIGHT].pos.y = g_penguin[i].penguin_body.pos.y - 1.0f + (float)sin(g_penguin[i].anim_move) / 4;
		g_penguin[i].penguin_leg[LEFT].pos.x = g_penguin[i].penguin_body.pos.x;
		g_penguin[i].penguin_leg[RIGHT].pos.x = g_penguin[i].penguin_body.pos.x;
		g_penguin[i].anim_move += 0.05f;

		//�s�񍇐�
		//��
		//g_penguin[i].penguin_body.mtxWorld = g_penguin[i].penguin_body.mtxMove * g_penguin[i].rot * g_penguin[i].penguin_body.mtxRotCenter * g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_body.mtxWorld;
		g_penguin[i].penguin_body.mtxWorld = g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_body.mtxMove * g_penguin[i].rot * g_penguin[i].penguin_body.mtxTrans;
		//��
		g_penguin[i].penguin_leg[g_penguin[i].leg_index].mtxWorld = g_penguin[i].penguin_body.mtxRot * g_penguin[i].penguin_leg[g_penguin[i].leg_index].mtxWorld;
		for (int j = 0; j < LEG_MAX; j++)
		{
			g_penguin[i].penguin_leg[j].mtxWorld = g_penguin[i].penguin_leg[j].mtxTransBody * g_penguin[i].penguin_leg[j].mtxSin * g_penguin[i].rot * g_penguin[i].penguin_leg[j].mtxWorld;
		}

		//�s�񍇐���̍��W�X�V
		g_penguin[i].penguin_body.pos.x = g_penguin[i].penguin_body.mtxWorld._41;
		g_penguin[i].penguin_body.pos.z = g_penguin[i].penguin_body.mtxWorld._43;
	}
}

//====================================================
//�`��
//====================================================
void Penguin_Draw(void)
{
	for (int i = 0; i < PENGUIN_MAX; i++)
	{
		//��
		Cube_Draw(&g_penguin[i].penguin_body.mtxWorld);
		//��
		for (int j = 0; j < LEG_MAX; j++)
		{
			Cube_Draw(&g_penguin[i].penguin_leg[j].mtxWorld);
		}
	}
	DebugFont_Draw(0, 32 * 3, "�� = %.02lf", sin(g_penguin[0].anim_move) / 8);
	DebugFont_Draw(0, 32 * 4, "�r = %.02lf", sin(g_penguin[0].anim_move) / 8);
}