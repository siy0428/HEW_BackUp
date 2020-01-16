//----------------------------------------------------
//	セーブ＆ロード用.cpp
//
//	クラス	：AT-12E-353
//	出席番号：16
//	名前	：木村　和
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
//定数定義
//----------------------
#define DATA_NAME "Accet/stage_date/data.stg"
#define FIlLENAME_MAX (128)

//----------------------
//グローバル変数
//----------------------
static int stageId;

//----------------------
//関数定義
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
		MessageBox(NULL, "ステージのロードに失敗しました", "エラー", MB_OK);
		return;
	}
	else {
		fread(stage_name, sizeof(char[FIlLENAME_MAX]), 1, fp1);
		fread(stage_path, sizeof(char[FIlLENAME_MAX]), 1, fp1);
		stageId = Model_Load(stage_name, stage_path);//modelID取得関数

		//ペンギンロード
		fread(&intWork1, sizeof(long), 1, fp1);
		fread(&intWork1, sizeof(long), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Penguin_Set(mtxWork1);
		}

		//白熊ロード
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			WhiteBear_Set(mtxWork1);
		}

		//つららロード
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Turara_Set(mtxWork1);
		}

		//かべロード
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			//SetKaba(&mtxWork1);
		}

		//スタートロード
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Stone_SetStart(mtxWork1);
		}

		//ゴールロード
		fread(&intWork1, sizeof(int), 1, fp1);
		fread(&intWork1, sizeof(int), 1, fp1);
		for (cnt2 = 0; cnt2 < intWork1; cnt2++) {
			fread(mtxWork1, sizeof(D3DXMATRIX), 1, fp1);
			Goal_Set(mtxWork1);
		}

		if (fclose(fp1) == EOF) {
			MessageBox(NULL, "ステージのロードに失敗しました", "エラー", MB_OK);
			return;
		}
	}
	return;
}

int GetStageId(void)
{
	return stageId;
}