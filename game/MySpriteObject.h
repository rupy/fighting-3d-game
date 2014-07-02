#ifndef __MYSPRITEOBJECT_H__
#define __MYSPRITEOBJECT_H__

#include "Object.h"
#include "Effect.h"
enum CenterPositionFlag{
	CPF_LEFT_TOP,
	CPF_LEFT_BOTTOM,
	CPF_CENTER,
	CPF_RIGHT_TOP,
	CPF_RIGHT_BOTTOM,
};
class MySpriteObject :public Object
{
public:
	LPDIRECT3DVERTEXBUFFER9		pMyVB;// ���_�o�b�t�@�[
	LPDIRECT3DTEXTURE9 pTex;
	bool myTex;
	int width;
	int height;
	D3DCOLOR color;
public:

	MySpriteObject(int width,int height,
		CenterPositionFlag center = CPF_LEFT_BOTTOM,
		LPDIRECT3DTEXTURE9 tex=NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0,0,0),
		float Y=0.0f,float R=0.0f,float P=0.0f);

	~MySpriteObject();
	//������`��
	void Appear();
	HRESULT SetupTexture(char* filename);
	HRESULT SetupTexture(char* filename,int width,int height);
protected:
		// ���_�f�[�^�̍\����
	struct SPRITE_VERTEX{
		D3DXVECTOR3 p;		// �ʒu
		float tu,tv;//�e�N�X�`��
	};
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
	#define SPRITE_VERTEX_FVF  (D3DFVF_XYZ|D3DFVF_TEX1 )
};
#endif	/*__MYSPRITEOBJECT_H__*/