#include <d3dx9.h>
#include "mydirectx.h"
#include "common.h"
#include "debug_font.h"
#include "input.h"
#include "joycon.h"
#include "JoyInput.h"
#include "mouse.h"
#include "camera.h"
#include "stone.h"
#include "goal.h"
#include "joyInput.h"
#include "power_gauge.h"
#include "game.h"
#include "result.h"
#include "model.h"
#include "OXAllocateHierarchy.h"

//====================================================
//�}�N����`
//====================================================
#define DEAD_ZONE (0.001f)	//�����_�덷�p

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

//====================================================
//�\���̐錾
//====================================================
typedef struct Stone_Vertex_tag
{
	D3DXVECTOR3 position;	//���W
	D3DCOLOR color;			//�F���
}StoneVertex;
#define FVF_CUBE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//�X�g�[���\����
typedef struct
{
	D3DXMATRIX mtx_world;		//���[���h�s��
	D3DXMATRIX mtx_rotation;	//��]�s��
	D3DXMATRIX mtx_trans;		//���s�ړ�
	D3DXMATRIX mtx_move;		//�ړ���
	D3DXVECTOR3 pos = { 0.0f, 0.5f, 0.0f };				//�ʒu���W
	D3DXVECTOR3 start_pos = { 0.0f, 0.5f, 0.0f };		//1F�O�̈ʒu���W
	D3DXCOLOR color = { 1.0f, 1.0f, 1.0f, 1.0f };		//�X�g�[���̐F���
	unsigned int model[STONE_TYPE_MAX];
	STONE_TYPE type;
	int type_index;
	float move;
	bool is_turn;
	float goal_range;
	float stone_range;
}Stone;

//=====================================================
//�O���[�o���ϐ�
//=====================================================
static const StoneVertex g_stone_vertex[] = {						//���_�\����
	//����
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 255, 255)},
	//�E��
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(0, 255, 0, 255)},
	//����
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(255, 0, 0, 255)},
	//����
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(64, 128, 0, 255)},
	//���
	{D3DXVECTOR3(0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, 0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	{D3DXVECTOR3(-0.5f, -0.5f, -0.5f), D3DCOLOR_RGBA(128, 32, 64, 255)},
	//���
	{D3DXVECTOR3(-0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(-0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, 0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)},
	{D3DXVECTOR3(0.5f, 0.5f, -0.5f), D3DCOLOR_RGBA(255, 255, 0, 255)}

};

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
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//���s�ړ�
		g_stone[i].move = 0.0f;			//�ړ���
		g_stone[i].is_turn = false;
		g_stone[i].goal_range = Goal_Range(g_stone[i].pos);
		g_stone[i].stone_range = 0.0f;
		g_stone[i].type = STONE_NORMAL;
		g_stone[i].type_index = g_stone[i].type;
		if (g_filename[i] != '\0')
		{
			g_stone[i].model[STONE_NORMAL] = Model_Load(g_filename[i * 3], g_pathname[i * 3]);			//�ʏ�^�C�v
			g_stone[i].model[STONE_HEAVY] = Model_Load(g_filename[1 + i * 3], g_pathname[1 + i * 3]);	//�d���^�C�v
			g_stone[i].model[STONE_FLOAT] = Model_Load(g_filename[2 + i * 3], g_pathname[2 + i * 3]);	//���V�^�C�v
		}
	}

	g_stone[0].is_turn = true;		//�ŏ��̃X�g�[���̃^�[���ݒ�
	g_stone[0].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);	//1P�}�e���A���J���[
	g_stone[1].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	//2P�}�e���A���J���[
	g_stone[2].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	//3P�}�e���A���J���[
	g_stone[3].color = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);	//4P�}�e���A���J���[
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

//=====================================================
//�X�V
//=====================================================
void Stone_Update(void)
{
	//�ǂ̃v���C���[�̃^�[��������
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		//�^�[�������ĂȂ���Ύ��̃v���C���[��
		if (!g_stone[i].is_turn)
		{
			continue;
		}

		//�X�g�[���؂�ւ�
		switch (g_stone[i].type_index)
		{
		case 0:
			g_stone[i].type = STONE_NORMAL;
			break;
		case 1:
			g_stone[i].type = STONE_HEAVY;
			break;
		case 2:
			g_stone[i].type = STONE_FLOAT;
			break;
		default:
			break;
		}

		//�X�g�[���؂�ւ�(��)
		if (GetButton_isTrigger(JC_Y) && !Stone_Move(i))
		{
			g_stone[i].type_index = (g_stone[i].type_index + 1) % 3;
		}
		//�X�g�[���؂�ւ�(�E)
		if (GetButton_isTrigger(JC_A) && !Stone_Move(i))
		{
			g_stone[i].type_index--;
			if (g_stone[i].type_index < 0)
			{
				g_stone[i].type_index = STONE_TYPE_MAX - 1;
			}
		}

		//�}�E�X�p�����[�^���Z�b�g
		if (GetButton_isTrigger(JC_X))
		{
			Mouse_Reset();
		}

		//�����ĂȂ�����Έړ��ʂ̐ݒ�
		if ((GetAsyncKeyState(VK_LBUTTON) || GetButton(JC_X)) && Stone_Range(g_stone[i].pos, g_stone[i].start_pos) <= DEAD_ZONE)
		{
			g_stone[i].move = 0.01f * Mouse_GetForce();
			rot_count = 0.075f * GetGyro().y / 1000;
			D3DXMatrixRotationY(&g_stone[i].mtx_rotation, Joycon_Operator() * D3DX_PI / 180);		//y����]
			Pow_Gauge_Update();
		}
		//�X�g�[���̈ړ�
		else
		{
			g_stone[i].move *= 0.98f;		//���C�W��
			D3DXMatrixTranslation(&g_stone[i].mtx_move, 0, 0, g_stone[i].move);
			g_stone[i].goal_range = Goal_Range(g_stone[i].pos);	//�S�[���܂ł̋����v�Z
			//�����Ă��Ȃ��Ƃ��̏���
			if (!g_throw_start)
			{
				Pow_Gauge_Update();
			}
			//�����Ă���Ƃ�
			if (g_stone[i].move >= DEAD_ZONE)
			{
				g_throw_start = true;
			}
			//�W���C����]�̃��Z�b�g
			else
			{
				rot_count = 0.0f;
			}
			//�ړ��ʂ�DEAD_ZONE�ȉ��Ŏ��̃^�[��
			if (g_stone[i].move <= DEAD_ZONE && g_throw_start)
			{
				//�S�[������
				if (Goal_Flag(g_stone[i].goal_range, g_stone[i].move))
				{
					Result_GetWinPlayer(g_player_turn);
				}
				g_stone[g_player_turn].is_turn = false;							//���ݑ��삵�Ă���X�g�[���̃^�[����false
				g_stone[g_player_turn].start_pos = g_stone[g_player_turn].pos;	//�X�^�[�g�ʒu�ɍ��̍��W����
				g_stone[g_player_turn].move = 0.0f;								//�ړ��ʂ̏�����
				g_turn_count++;	//�^�[�����J�E���g
				(g_turn_count < 4) ? g_player_turn++ /*= (g_player_turn + 1) % 4*/ : g_player_turn = Stone_Turn();	//1���ڂ�1�`4P�̏��ŁA����ȍ~�̓S�[�����牓����
				g_stone[g_player_turn].is_turn = true;							//�v���X���ꂽ���݂̃v���C���[�̃^�[����true��
				g_throw_start = false;											//�X�^�[�g�p�ϐ���false
				Joycon_Reset();			//�X�e�B�b�N��]�l���Z�b�g
				rot_count = 0.0f;		//��]�ʃ��Z�b�g
				gyro_rotation = 0.0f;	//��]�l���Z�b�g
			}
			gyro_rotation += rot_count;
			rot_count *= 0.85f;
			D3DXMatrixRotationY(&g_stone[i].mtx_rotation, (Joycon_Operator() * D3DX_PI / 180) + gyro_rotation);	//y����]
		}

		//�X�g�[���̐i�񂾋���
		g_stone[i].stone_range = Stone_Range(g_stone[i].pos, g_stone[i].start_pos);

		//���[���h���W�ϊ�
		D3DXMatrixIdentity(&g_stone[i].mtx_world);	//�P�ʍs������
		D3DXMatrixTranslation(&g_stone[i].mtx_world, g_stone[i].pos.x, g_stone[i].pos.y, g_stone[i].pos.z);		//���s�ړ�
		g_stone[i].mtx_world = g_stone[i].mtx_move * g_stone[i].mtx_rotation * g_stone[i].mtx_world;

		//�s�񍇐�������Ԃł̍��W�擾
		g_stone[i].pos.x = g_stone[i].mtx_world._41;
		g_stone[i].pos.y = g_stone[i].mtx_world._42;
		g_stone[i].pos.z = g_stone[i].mtx_world._43;
	}
}

//=====================================================
//�`��
//=====================================================
void Stone_Draw(void)
{
	//DebugFont_Draw(0, 32 * 15, "A�g���K�[ = %d", GetButton_isTrigger(JC_A));
	//DebugFont_Draw(0, 32 * 16, "Y�g���K�[ = %d", GetButton_isTrigger(JC_Y));
	//DebugFont_Draw(0, 32 * 17, "X�g���K�[ = %d", GetButton_isTrigger(JC_X));
	DebugFont_Draw(0, 32 * 17, "Y�v���X = %d", GetButton(JC_Y));
	for (int i = 0; i < PLAYER_MAX_NUM; i++)
	{
		DebugFont_Draw(0, 32 * (6 + i), "%dP�̃S�[���܂ł̋��� = %.02lf", i + 1, g_stone[i].goal_range);
		//DebugFont_Draw(0, 32 * (6 + i), "%dP�̐i�񂾋��� = %.02lf", i + 1, g_stone[i].stone_range);

		Model_Draw(g_stone[i].model[g_stone[i].type], g_stone[i].mtx_world);
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