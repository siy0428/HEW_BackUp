//=======================================================================
//							�e�N�X�`��
//
//	�o�Ȕԍ� 45	
//	�R�� ����
//=======================================================================

#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirectx.h"
#include "texture.h"

typedef struct TextureDate_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;

//===========================================
//�O���[�o���ϐ�
//===========================================
static TextureData g_TextureData[TEXTUREDATA_MAX] = {};

//===========================================
//�e�N�X�`���̋󂫂�����
//===========================================
int Texture_SetLoadFile(const char* pFilename, int width, int height)
{
	//�����t�@�C������T��
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//�����t�@�C�������q�b�g�����Ƃ��̏���
		if (strcmp(g_TextureData[i].filename, pFilename) == 0)
		{
			return i;	//�q�b�g�����Ǘ��ԍ���Ԃ�
		}
	}
	//�V�K�o�^
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//���O�����łɓ����Ă����玟�̃��[�v
		if (g_TextureData[i].filename[0] != '\0')
		{
			continue;
		}
		//�o�^
		strcpy(g_TextureData[i].filename, pFilename);	//���O�̃R�s�[
		g_TextureData[i].width = width;					//���̃R�s�[
		g_TextureData[i].height = height;				//�����̃R�s�[

		return i;
	}
	return -1;	//�󂫕s��
}

//===========================================
//�e�N�X�`���̓ǂݍ���
//===========================================
int Texture_Load()
{
	int error_count = 0;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		//�t�@�C�������ݒ肳��Ă�Ύ��̃��[�v
		if (g_TextureData[i].filename[0] == '\0')
		{
			continue;
		}

		//�ǂݍ��܂ꂽ�e�N�X�`���͎��̃��[�v
		if (g_TextureData[i].pTexture != NULL)
		{
			error_count++;
			continue;
		}

		//�e�N�X�`���̎擾
		D3DXCreateTextureFromFile(pDevice, g_TextureData[i].filename, &g_TextureData[i].pTexture);

	}
	return error_count;
}

//===========================================
//�e�N�X�`�����擾
//===========================================
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id)
{
	return g_TextureData[id].pTexture;
}

//===========================================
//�e�N�X�`���̕����擾
//===========================================
int Texture_GetWidth(int id)
{
	return g_TextureData[id].width;
}

//===========================================
//�e�N�X�`���̍������擾
//===========================================
int Texture_GetHeight(int id)
{
	return g_TextureData[id].height;
}

//===========================================
//�w�肵���e�N�X�`��������
//===========================================
void Texture_Destroy(int ids[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (g_TextureData[ids[i]].pTexture != NULL)
		{
			g_TextureData[ids[i]].pTexture->Release();
			g_TextureData[ids[i]].pTexture = NULL;
			g_TextureData[ids[i]].filename[0] = '\0';
		}
	}
}

//===========================================
//�e�N�X�`�������ׂď���
//===========================================
void Texture_Destroy(void)
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].pTexture != NULL)
		{
			g_TextureData[i].pTexture->Release();
			g_TextureData[i].pTexture = NULL;
			g_TextureData[i].filename[0] = '\0';
		}
	}
}