#ifndef __FONT_H__
#define __FONT_H__

#include <d3dx9.h>
class Font
{
public:
	LPDIRECT3DTEXTURE9 pTex;
	float fTexW; // �e�N�X�`���̉���
	float fTexH; // �e�N�X�`���̍���
	//�f�t�H���g�R���X�g���N�^
	Font();
	//�f�X�g���N�^
	~Font();

	HRESULT MakeFont(LPDIRECT3DDEVICE9 device,const TCHAR* ch,int fontsize);
};

#endif	/*__FONT_H__*/