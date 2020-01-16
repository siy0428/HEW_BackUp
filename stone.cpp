#include "common.h"
#include "debug_font.h"

//���͊֌W
#include "input.h"
#include "joycon.h"
#include "JoyInput.h"
#include "mouse.h"


#include "camera.h"
#include "stone.h"
#include "goal.h"
#include "power_gauge.h"
#include "game.h"
#include "result.h"
#include "model.h"
#include "OXAllocateHierarchy.h"
#include "ui_player_turn.h"
#include "fade.h"
#include "penguin.h"

//====================================================
//�}�N����`
//====================================================
#define DEAD_ZONE (0.001f)	//�����_�덷�p


//����
#define TEIMEN (0.0f)
#define MASATU (0.5f)
#define JUURYOKU ( 9.8f)
#define GENSOKU (1.02f)

#define SPEED (0.01f)

//====================================================
//�񋓌^
//====================================================
typedef enum
{
	STONE_NORMAL,	//�ʏ�
	STONE_HEAVY,	//�d��
	STONE_FLOAT,	//���V
	STONE_TYPE_MAX
}STONE_TYPE;

//=====================================================
//�\���̐錾
//=====================================================
typedef struct
{
	D3DXMATRIX mtx_world;		//���[���h�s��
	D3DXMATRIX mtx_rotation;	//��]�s��
	D3DXMATRIX mtx_trans;		//���s�ړ�
	D3DXMATRIX mtx_move;		//�ړ���
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };				//�ʒu���W
	D3DXVECTOR3 start_pos = { 0.0f, 0.5f, 0.0f };		//1F�O�̈ʒu���W
	D3DXVECTOR3 vecSpeed = { 0.0f,0.0f,0.0f };
	unsigned int model[STONE_TYPE_MAX];
	STONE_TYPE type;
	int type_index;
	float move;
	float rot_move;
	bool is_turn;
	bool isUse;
	float goal_range;
	float stone_range;
	int score;
	int rank;
}Stone;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static Stone g_stone[PLAYER_MAX_NUM];
static int g_player_turn = 0;
static bool g_throw_start = false;
static float gyro_rotation = 0.0f;
static float rot_count = 0;
static int g_turn_count = 0;
static const char g_filename[16][256]
{
	//1P
	"model\\stone\\stone1p\\stone1p.x",
	"model\\stone\\omoi1p\\omoi1p.x",
	"model\\stone\\uiteru1p\\uiteru1p.x",
	//2P
	"model\\stone\\stone2p\\stone2p.x",
	"model\\stone\\omoi2p\\omoi2p.x",
	"model\\stone\\uiteru2p\\uiteru2p.x",
	//3P
	"model\\stone\\stone3p\\stone3p.x",
	"model\\stone\\omoi3p\\omoi3p.x",
	"model\\stone\\uiteru3p\\uiteru3p.x",
	//4P
	"model\\stone\\stone4p\\stone4p.x",
	"model\\stone\\omoi4p\\omoi4p.x",
	"model\\stone\\uiteru4p\\uiteru4p.x"
};
static const char g_pathname[16][256]
{
	//1P
	"model\\stone\\stone1p\\",
	"model\\stone\\omoi1p\\",
	"model\\stone\\uiteru1p\\",
	//2P
	"model\\stone\\stone2p\\",
	"model\\stone\\omoi2p\\",
	"model\\stone\\uiteru2p\\",
	//3P
	"model\\stone\\stone3p\\",
	"model\\stone\\omoi3p\\",
	"model\\stone\\uiteru3p\\",
	//4P
	"model\\stone\\stone4p\\",
	"model\\stone\\omoi4p\\",
	"model\\stone\\uiteru4p\\"
};

//=====================================================
//������
//=====================================================
void Stone_Init(void)
{
	//�X�g�[���̏�����
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_stone[i].pos = D3DXVECTOR3(-5.0f + i * 2.0f, 0.5f, -10.0f);	//���W
		g_stone[i].start_pos = g_stone[i].pos;
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);	//���s�ړ�
		g_stone[i].move = 0.0f;			//�ړ���
		g_stone[i].rot_move = 0.0f;
		g_stone[i].is_turn = false;
		g_stone[i].goal_range = Goal_Range(g_stone[i].pos);
		g_stone[i].stone_range = 0.0f;
		g_stone[i].type = STONE_NORMAL;
		g_stone[i].type_index = g_stone[i].type;
		g_stone[i].score = -1;
		g_stone[i].isUse = false;
		if (g_filename[i] != '\0')
		{
			g_stone[i].model[STONE_NORMAL] = Model_Load(g_filename[i * 3], g_pathname[i * 3]);			//�ʏ�^�C�v
			g_stone[i].model[STONE_HEAVY] = Model_Load(g_filename[1 + i * 3], g_pathname[1 + i * 3]);	//�d���^�C�v
			g_stone[i].model[STONE_FLOAT] = Model_Load(g_filename[2 + i * 3], g_pathname[2 + i * 3]);	//���V�^�C�v
		}
	}
	g_stone[0].is_turn = true;		//�ŏ��̃X�g�[���̃^�[���ݒ�
	g_stone[0].isUse = true;
	g_player_turn = 0;// Stone_Turn();
	g_throw_start = false;
	gyro_rotation = 0.0f;
	g_turn_count = 0;
}

//=====================================================
//�I��
//=====================================================
void Stone_Uninit(void)
{

}
static int click;
//=====================================================
//�X�V
//=====================================================
void Stone_Update(void)
{
	//�f�o�b�O�p�V�[���؂�ւ�
#if (DEBUG) || (_DEBUG)
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		for (int j = 0; j < 4; j++)
		{
			Result_SetScore(j, g_stone[j].score);
		}
		Result_GetRanking();	//���ʃ\�[�g
		Goal_GetWinPlayer(g_player_turn);
		Fade_InOut(SCENE_RESULT);
	}
#endif
	//�ǂ̃v���C���[�̃^�[��������
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//�^�[�������ĂȂ���Ύ��̃v���C���[��
		if (!g_stone[i].is_turn)
		{
			continue;
		}

		//UI��������܂ő���s�\
		if (Ui_PlayerTurn_GetType() == UI_FIRST_TURN || Ui_PlayerTurn_GetNextType() == UI_FIRST_TURN ||
			Ui_PlayerTurn_GetType() == UI_STOP || Ui_PlayerTurn_GetNextType() == UI_STOP)
		{
			return;
		}

		////�X�g�[���؂�ւ�
		//switch (g_stone[i].type_index)
		//{
		//case 0:
		//	g_stone[i].type = STONE_NORMAL;
		//	break;
		//case 1:
		//	g_stone[i].type = STONE_HEAVY;
		//	break;
		//case 2:
		//	g_stone[i].type = STONE_FLOAT;
		//	break;
		//default:
		//	break;
		//}

		//�X�g�[���؂�ւ�(��)
		if (GetButton_isTrigger(JC_Y) && !Stone_Move(i) && !Penguin_OnStone(i))
		{
			//g_stone[i].type_index = (g_stone[i].type_index + 1) % 3;
			g_stone[i].type = (STONE_TYPE)(((int)g_stone[i].type + 1) % 3);
			Ui_PlayerTurn_StoneChange(false);
		}
		//�X�g�[���؂�ւ�(�E)
		if (GetButton_isTrigger(JC_A) && !Stone_Move(i) && !Penguin_OnStone(i))
		{
			//g_stone[i].type_index--;
			//if (g_stone[i].type_index < 0)
			//{
			//	g_stone[i].type_index = STONE_TYPE_MAX - 1;
			//}
			g_stone[i].type = (STONE_TYPE)((int)g_stone[i].type - 1);
			if ((int)g_stone[i].type < 0)
			{
				g_stone[i].type = (STONE_TYPE)(int)(STONE_TYPE_MAX - 1);
			}
			Ui_PlayerTurn_StoneChange(true);
		}

		//�}�E�X�p�����[�^���Z�b�g
		if (GetButton_isTrigger(JC_X))
		{
			Mouse_Reset();
		}
		click = (bool)GetAsyncKeyState(VK_LBUTTON);
		//�����ĂȂ�����Έړ��ʂ̐ݒ�
		if (((bool)GetAsyncKeyState(VK_LBUTTON) || GetButton(JC_X)) && Stone_Range(g_stone[i].pos, g_stone[i].start_pos) <= DEAD_ZONE)
		{
			g_stone[i].move = 0.01f * Mouse_GetForce();
			rot_count = 0.075f * GetGyro().y / 1000;
			Pow_Gauge_Update();
			Ui_PlayerTurn_Create(UI_RANGE);
			D3DXMatrixRotationY(&g_stone[i].mtx_rotation, gyro_rotation);	//�W���C����]
		}
		//�X�g�[���̈ړ�
		else
		{
			g_stone[i].rot_move = Joycon_Operator() * D3DX_PI / 180;	//��]�l
			g_stone[i].move *= 0.98f;		//���C�W��
			g_stone[i].goal_range = Goal_Range(g_stone[i].pos);	//�S�[���܂ł̋����v�Z
			//�����Ă��Ȃ��Ƃ��̏���
			if (!g_throw_start)
			{
				g_stone[i].start_pos = g_stone[i].pos;
				Ui_PlayerTurn_Create(UI_DIR_CHANGE);
				Pow_Gauge_Update();
			}
			//�����Ă���Ƃ�
			if (g_stone[i].move >= DEAD_ZONE)
			{
				//������Ƃ�UI
				g_throw_start = true;
				Ui_PlayerTurn_Create(UI_RANGE);
			}
			//�W���C����]�̃��Z�b�g
			else
			{
				rot_count = 0.0f;
			}
			//�ړ��ʂ�DEAD_ZONE�ȉ��Ŏ��̃^�[��
			if (g_stone[i].move <= DEAD_ZONE && g_throw_start)
			{
				g_stone[i].move = 0.0f;
				//�S�[������
				if (Goal_Flag(g_stone[i].goal_range, g_stone[i].move))
				{
					for (int j = 0; j < 4; j++)
					{
						Result_SetScore(j, g_stone[i].score);
					}
					Result_GetRanking();	//���ʃ\�[�g
					Goal_GetWinPlayer(g_player_turn);
					Fade_InOut(SCENE_RESULT);
					return;
				}
				Ui_PlayerTurn_Create(UI_STOP);
			}
			gyro_rotation += rot_count;
			rot_count *= 0.85f;
			D3DXMatrixRotationY(&g_stone[i].mtx_rotation, D3DXToRadian(Joycon_Operator()));	//�X�e�B�b�N��]

			//�X�g�[���̐i�񂾋���
			g_stone[i].stone_range = Stone_Range(g_stone[i].pos, g_stone[i].start_pos);

			//���[���h���W�ϊ�
			D3DXVECTOR3 vecWork(0.0f, 0.0f, g_stone[i].move);
			D3DXVec3TransformNormal(&vecWork, &vecWork, &g_stone[i].mtx_rotation);

			//�n�`�ω�
			BOOL hit;
			bool x, z;

			g_stone[i].pos.y = TEIMEN;
			D3DXVECTOR3 Nvec = collisionNormal(&g_stone[i].pos, &hit, &g_stone[i].pos.y, &x, &z);
			g_stone[i].pos.y += TEIMEN;
			if (FALSE == hit) {
				//���b�V���̂����W���}�C�i�X�̒l�ɂȂ��Ă���
				g_stone[i].pos.y = TEIMEN;
			}
			if (x) { Nvec.x = 0.0f; }
			if (z) { Nvec.z = 0.0f; }
			//katamuki
			float m_masatu;

			D3DXVECTOR3 juuryoku = Nvec;
			D3DXVECTOR3 force, yoko;
			D3DXVECTOR3 under = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&juuryoku, &juuryoku);
			float sin_saka = sqrt((juuryoku.x)*(juuryoku.x) + (juuryoku.z)*(juuryoku.z));
			float tan_saka;
			if (0.0f == juuryoku.y) {
				tan_saka = 0.0f;
			}
			else {
				tan_saka = sin_saka / juuryoku.y;
			}



			if (MASATU <= tan_saka) {
				//����
				m_masatu = (juuryoku.y - sin_saka * MASATU)*JUURYOKU;
			}
			else {
				//�~�܂�
				m_masatu = 0;
			}

			//syuusei
			D3DXVec3Cross(&yoko, &juuryoku, &under);
			D3DXVec3Cross(&force, &yoko, &juuryoku);
			if (force.y > 0.0f) {
				force = -force;
			}
			D3DXVec3Normalize(&force, &force);

			force *= m_masatu;
			//force.y = 0.0f;

			//�ړ�
			g_stone[i].vecSpeed += SPEED * vecWork + force;

			g_stone[i].vecSpeed += force * 0.01f;
			g_stone[i].vecSpeed /= GENSOKU;
			g_stone[i].pos += g_stone[i].vecSpeed;

			D3DXMatrixTranslation(&g_stone[i].mtx_trans, g_stone[i].vecSpeed.x, g_stone[i].vecSpeed.y, g_stone[i].vecSpeed.z);

			g_stone[i].mtx_world = g_stone[i].mtx_rotation * g_stone[i].mtx_trans * g_stone[i].mtx_world;

			//�s�񍇐�������Ԃł̍��W�擾
			g_stone[i].pos.x = g_stone[i].mtx_world._41;
			g_stone[i].pos.y = g_stone[i].mtx_world._42;
			g_stone[i].pos.z = g_stone[i].mtx_world._43;
		}
	}
}

//=====================================================
//�`��
//=====================================================
void Stone_Draw(void)
{
	D3DXMATRIX mtxWork[2];
	D3DXMatrixRotationY(&mtxWork[0], D3DX_PI);
	DebugFont_Draw(0, 32 * 20, "�N���b�N = %d", click);
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		if (!g_stone[i].isUse)
		{
			continue;
		}
		DebugFont_Draw(0, 32 * (6 + i), "%dP�̃S�[���܂ł̋��� = %.02lf", i + 1, g_stone[i].goal_range);
		//DebugFont_Draw(0, 32 * (6 + i), "%dP�̐i�񂾋��� = %.02lf", i + 1, g_stone[i].stone_range);
		mtxWork[1] = mtxWork[0] * g_stone[i].mtx_world;
		
		Model_Draw(g_stone[i].model[g_stone[i].type], mtxWork[1]);
	}
}

//=====================================================
//���W�擾
//=====================================================
D3DXVECTOR3 Stone_GetPos(int index)
{
	return g_stone[index].pos;
}

//=====================================================
//��P�����쒆��
//=====================================================
int Stone_PlayerTurn(void)
{
	return g_player_turn;
}

//=====================================================
//�X�g�[���̐i�񂾋���
//=====================================================
float Stone_Range(D3DXVECTOR3 pos, D3DXVECTOR3 start_pos)
{
	return (float)sqrt((pos.x - start_pos.x) * (pos.x - start_pos.x) + (pos.y - start_pos.y) * (pos.y - start_pos.y) + (pos.z - start_pos.z) * (pos.z - start_pos.z));
}

//=====================================================
//�X�g�[���̐i�񂾋����̎擾
//=====================================================
float Stone_GetRange(int index)
{
	return g_stone[index].stone_range;
}

//=====================================================
//�X�g�[���������Ă��邩�ǂ���
//=====================================================
bool Stone_Move(int index)
{
	//�X�g�[���̈ړ��ʂ�DEAD_ZONE�ȉ��ł���ΐÎ~���Ɣ��f
	if (g_stone[index].move <= DEAD_ZONE)
	{
		return false;
	}
	return true;
}

//=====================================================
//�S�[���̋����ɉ����ăX�g�[���̏��ԏ���
//=====================================================
int Stone_Turn(void)
{
	int index = 0;

	for (int i = 1; i < PLAYER_MAX_NUM; i++)
	{
		//���̔�r�Ώۂ̕����S�[����艓���ꍇ
		if (Goal_Range(g_stone[i].pos) > Goal_Range(g_stone[index].pos))
		{
			index = i;
		}
	}
	return index;
}

//=====================================================
//��]�ʎ擾
//=====================================================
float Stone_Rot(int index)
{
	return g_stone[index].rot_move;
}

//=====================================================
//���̃X�g�[���̐ݒ�
//=====================================================
void Stone_SetTurn(void)
{
	g_stone[g_player_turn].is_turn = false;									//���ݑ��삵�Ă���X�g�[���̃^�[����false
	g_stone[g_player_turn].start_pos = g_stone[g_player_turn].pos;			//�X�^�[�g�ʒu�ɍ��̍��W����
	g_stone[g_player_turn].move = 0.0f;										//�ړ��ʂ̏�����
	g_stone[g_player_turn].score++;											//�X�R�A�̉��Z
	g_turn_count++;			//�^�[�����J�E���g
	(g_turn_count < 4) ? g_player_turn++ : g_player_turn = Stone_Turn();	//1���ڂ�1�`4P�̏��ŁA����ȍ~�̓S�[�����牓����
	g_stone[g_player_turn].is_turn = true;									//�v���X���ꂽ���݂̃v���C���[�̃^�[����true��
	g_stone[g_player_turn].isUse = true;
	g_throw_start = false;													//�X�^�[�g�p�ϐ���false
	rot_count = 0.0f;		//��]�ʃ��Z�b�g
	gyro_rotation = 0.0f;	//��]�l���Z�b�g
}

//=====================================================
//�X�R�A�̎擾
//=====================================================
int Stone_GetScore(int index)
{
	return g_stone[index].score;
}

//=====================================================
//�R�[�X�G�f�B�^�ǂݍ���
//=====================================================
void Stone_SetStart(D3DXMATRIX mtxWorld)
{
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		g_stone[i].mtx_world = mtxWorld;
		g_stone[i].pos.x = mtxWorld._41;
		g_stone[i].pos.y = mtxWorld._42;
		g_stone[i].pos.z = mtxWorld._43;
		g_stone[i].start_pos = g_stone[i].pos;
	}
}

//=====================================================
//�X�g�[���s��ǂݍ���
//=====================================================
D3DXMATRIX Stone_GetMtx(int index)
{
	return g_stone[index].mtx_world;
}