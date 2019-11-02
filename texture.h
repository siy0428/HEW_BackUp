
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <d3d9.h>

int Texture_SetLoadFile(const char* pFilename, int width, int height);	//(�߂�l)�\��ԍ� �ǂݍ��ރt�@�C���̗\��(�t�@�C����,�e�N�X�`���̕�,����)
int Texture_Load();														//(�߂�l)�ǂݍ��߂Ȃ������� �t�@�C���̓ǂݍ���
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);							//(�߂�l)�e�N�X�`���̔ԍ� �\�񂵂��e�N�X�`�����擾
int Texture_GetWidth(int id);
int Texture_GetHeight(int id);
void Texture_Destroy(int ids[], int count);								//�e�N�X�`����I�����ĉ��(����������e�N�X�`����id�̐擪�A�h���X, �����A�h���X����I��)
void Texture_Destroy(void);												//�e�N�X�`�������ׂĉ��
int Texture_GetWidth(int id);											//(�߂�l)�e�N�X�`���̕� �e�N�X�`���̗\��ԍ�
int Texture_GetHeight(int id);											//(�߂�l)�e�N�X�`���̍��� �e�N�X�`���̗\��ԍ�

#define TEXTUREDATA_MAX (64)	//�e�N�X�`���̍ő吔
#define FILENAME_MAX (128)		//�t�@�C�����̍ő啶����

#endif