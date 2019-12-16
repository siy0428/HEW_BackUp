#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3dx9.h>

int Model_Load(const char *filename, const char *path_name);
void Model_Uninit(void);
void Model_Draw(int modelId, D3DMATRIX mtxWorld);
void Model_Draw(int modelId, D3DMATRIX mtxWorld, D3DXCOLOR color);

#endif