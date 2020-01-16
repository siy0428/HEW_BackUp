//----------------------------------------------------
//	�Z�[�u�����[�h�p.cpp
//
//	�N���X	�FAT-12E-353
//	�o�Ȕԍ��F16
//	���O	�F�ؑ��@�a
//
//----------------------------------------------------

#include <stdio.h>
#include "mydirectx.h"
#include "model.h"
#include "penguin.h"
#include "white_bear.h"
#include "goal.h"
#include "stone.h"
#include "turara.h"

//----------------------
//�萔��`
//----------------------
#define DATA_NAME "Accet/stage_date/data.stg"
#define FIlLENAME_MAX (128)

//----------------------
//�O���[�o���ϐ�
//----------------------
static int stageId;

//----------------------
//�֐���`
//----------------------
void LoadStageDate(void)
{
	FILE *fp1;
	char stage_name[FIlLENAME_MAX];
	char stage_path[FIlLENAME_MAX];

	int cnt1, cnt2;
	int intWork1 = 0, intWork2 = 0;
	D3DXMATRIX mtxWork1;

	if ((fp1 = fopen(DATA_NAME, "rb")) == NULL) {
		MessageBox(NULL, "�X�e�[�W�̃��[�h�Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}
	else {
		fread(stage_name, sizeof(char[FIlLENAME_MAX]), 1, fp1);
		fread(stage_path, sizeof(char[FIlLENAME_MAX]), 1, fp1);
		stageId = Model_Load(stage_name, stage_path);//modelID�擾�֐�

		//�y���M�����[�h
		fread(&intWork1, sizeof(long), 1, fp1);
		fread(&intWork1, sizeof(long), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Penguin_Set(mtxWork1);
		}

		//���F���[�h
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			WhiteBear_Set(mtxWork1);
		}

		//��烍�[�h
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Turara_Set(mtxWork1);
		}

		//���׃��[�h
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			//SetKaba(&mtxWork1);
		}

		//�X�^�[�g���[�h
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Stone_SetStart(mtxWork1);
		}

		//�S�[�����[�h
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Goal_Set(mtxWork1);
		}

		if (fclose(fp1) == EOF) {
			MessageBox(NULL, "�X�e�[�W�̃��[�h�Ɏ��s���܂���", "�G���[", MB_OK);
			return;
		}
	}
	return;
}

int GetStageId(void)
{
	return stageId;
}