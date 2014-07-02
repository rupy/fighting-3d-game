#include "MySpriteObject.h"
#include "Common.h"
#include "Error.h"

MySpriteObject::MySpriteObject(int width,int height,
						   CenterPositionFlag center,
						   LPDIRECT3DTEXTURE9 tex,
						   D3DXVECTOR3 position,
						   float Y,float R,float P)
					   :Object(position,Y,R,P,D3DXVECTOR3((float)width,(float)height,1.0f)),
					   width(width),height(height),
					   pTex(tex),myTex(false),color(D3DXCOLOR(255))
{
	// 4�̒��_����Ȃ钸�_�o�b�t�@�����
	if( FAILED( GetDevice()->CreateVertexBuffer( 
		4*sizeof(SPRITE_VERTEX),	// ���_�o�b�t�@�S�̂̃T�C�Y
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g����
        SPRITE_VERTEX_FVF,			// ���_�f�[�^��FVF�t�H�[�}�b�g
        D3DPOOL_MANAGED, 		// �V�X�e���̒��_�o�b�t�@������
		&pMyVB, 				// ���_�o�b�t�@���i�[����ϐ�
		NULL ) ) )				// ���NULL�Ɏw��
	{
        return;
	}


    //���_�o�b�t�@�[�̒��g�𖄂߂�
    SPRITE_VERTEX* v;
    pMyVB->Lock( 0, 0, (void**)&v, 0 );

	switch(center){
		case CPF_LEFT_TOP:
			v[0].p = D3DXVECTOR3(0		,-1.0f	,0.0f );	// ���_�@
			v[1].p = D3DXVECTOR3(0		,0.0f	,0.0f );	// ���_�A
			v[2].p = D3DXVECTOR3(1.0f	,-1.0f	,0.0f );	// ���_�B
			v[3].p = D3DXVECTOR3(1.0f	,0.0f	,0.0f );	// ���_�C
			break;
		default:
		case CPF_LEFT_BOTTOM:
			v[0].p = D3DXVECTOR3(0		,0		,0.0f );	// ���_�@
			v[1].p = D3DXVECTOR3(0		,1.0f	,0.0f );	// ���_�A
			v[2].p = D3DXVECTOR3(1.0f	,0		,0.0f );	// ���_�B
			v[3].p = D3DXVECTOR3(1.0f	,1.0f	,0.0f );	// ���_�C
			break;
		case CPF_CENTER:
			v[0].p = D3DXVECTOR3(-0.5f, -0.5f, 0.0f );	// ���_�@
			v[1].p = D3DXVECTOR3(-0.5f,  0.5f, 0.0f );	// ���_�A
			v[2].p = D3DXVECTOR3( 0.5f, -0.5f, 0.0f );	// ���_�B
			v[3].p = D3DXVECTOR3( 0.5f,  0.5f, 0.0f );	// ���_�C
			break;
		case CPF_RIGHT_TOP:
			v[0].p = D3DXVECTOR3(-1.0f	,-1.0f	,0.0f );	// ���_�@
			v[1].p = D3DXVECTOR3(-1.0f	,0.0f	,0.0f );	// ���_�A
			v[2].p = D3DXVECTOR3(0.0f	,-1.0f	,0.0f );	// ���_�B
			v[3].p = D3DXVECTOR3(0.0f	,0.0f	,0.0f );	// ���_�C
			break;
		case CPF_RIGHT_BOTTOM:
			v[0].p = D3DXVECTOR3(-1.0f	,0.0f	,0.0f );	// ���_�@
			v[1].p = D3DXVECTOR3(-1.0f	,1.0f	,0.0f );	// ���_�A
			v[2].p = D3DXVECTOR3(0.0f	,0.0f	,0.0f );	// ���_�B
			v[3].p = D3DXVECTOR3(0.0f	,1.0f	,0.0f );	// ���_�C
			break;
	}

	
	//���_�f�[�^�F���_�̍��W�l�̐ݒ�
    v[0].tu = 0.0f;	// ���_�@
    v[0].tv = 1.0f;	// ���_�@
    v[1].tu = 0.0f;	// ���_�A
    v[1].tv = 0.0f;	// ���_�A
    v[2].tu = 1.0f;	// ���_�B
    v[2].tv = 1.0f;	// ���_�B
    v[3].tu = 1.0f;	// ���_�C
    v[3].tv = 0.0f;	// ���_�C

    
	//���_�f�[�^�F���_�̊�{�I�ȐF�̐ݒ�i���F�ɐݒ�j
	//v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
  
    pMyVB->Unlock();
}
MySpriteObject::~MySpriteObject(){
	if(myTex){
		SAFE_RELEASE(pTex);
	}
}



void MySpriteObject::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);
	GetDevice()->SetTexture(0,pTex);

	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(color);
	GetDevice()->SetMaterial(&mat);

	// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
	GetDevice()->SetStreamSource( 0, pMyVB, 0, sizeof(SPRITE_VERTEX) );

	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	GetDevice()->SetFVF( SPRITE_VERTEX_FVF );

	// �`��
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
HRESULT MySpriteObject::SetupTexture(char* filename){

	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFile(GetDevice(), filename,&pTex))){
			DisplayErrMsg( "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���");
			return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "���łɃe�N�X�`�����o�^����Ă���2D�I�u�W�F�N�g�Ƀe�N�X�`����ǂݍ������Ƃ��܂����B");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}HRESULT MySpriteObject::SetupTexture(char* filename,int width,int height){

	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFileEx( GetDevice(), filename,width,height,
			0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&pTex))){
				DisplayErrMsg( "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���");
				return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "���łɃe�N�X�`�����o�^����Ă���2D�I�u�W�F�N�g�Ƀe�N�X�`����ǂݍ������Ƃ��܂����B");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}