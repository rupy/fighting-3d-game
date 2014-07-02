#ifndef __VERTEXOBJECT_H__
#define __VERTEXOBJECT_H__

#include "Object.h"
#include "Effect.h"

class VertexObject :public Object
{
public:
	LPDIRECT3DVERTEXBUFFER9		pMyVB;// ���_�o�b�t�@�[
	LPDIRECT3DTEXTURE9 pTex;
	bool myTex;
	vector<Effect*>&	rEff;

	float alpha;
public:

	VertexObject(vector<Effect*>& eff,
		LPDIRECT3DTEXTURE9 tex=NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0,0,0),
		float Y=0.0f,float R=0.0f,float P=0.0f);

	~VertexObject();
	//������`��
	void Appear();
	HRESULT SetupTexture(char* filename);
	HRESULT SetupTexture(char* filename,int width,int height);
//private:
		// ���_�f�[�^�̍\����
	struct MY_VERTEX{
		D3DXVECTOR3 p;		// �ʒu
		float tu,tv;//�e�N�X�`��
	};
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
	#define MY_VERTEX_FVF  (D3DFVF_XYZ|D3DFVF_TEX1 )
};
#endif	/*__VERTEXOBJECT_H__*/