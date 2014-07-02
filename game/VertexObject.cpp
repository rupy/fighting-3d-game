#include "VertexObject.h"
#include "Error.h"

VertexObject::VertexObject(vector<Effect*>& eff,
						   LPDIRECT3DTEXTURE9 tex,
						   D3DXVECTOR3 position,
						   float Y,float R,float P)
					   :Object(position,Y,R,P),
					   rEff(eff),myTex(false),alpha(1.0f)
{
	this->pTex=tex;
	// 4�̒��_����Ȃ钸�_�o�b�t�@�����
	if( FAILED( GetDevice()->CreateVertexBuffer( 
		4*sizeof(MY_VERTEX),	// ���_�o�b�t�@�S�̂̃T�C�Y
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g����
        MY_VERTEX_FVF,			// ���_�f�[�^��FVF�t�H�[�}�b�g
        D3DPOOL_MANAGED, 		// �V�X�e���̒��_�o�b�t�@������
		&pMyVB, 				// ���_�o�b�t�@���i�[����ϐ�
		NULL ) ) )				// ���NULL�Ɏw��
	{
        return;
	}


    //���_�o�b�t�@�[�̒��g�𖄂߂�
    MY_VERTEX* v;
    pMyVB->Lock( 0, 0, (void**)&v, 0 );

	//���_�f�[�^�F���_�̍��W�l�̐ݒ�
	v[0].p = D3DXVECTOR3(-0.5f, -0.5f, 0.0f );	// ���_�@
	v[1].p = D3DXVECTOR3(-0.5f,  0.5f, 0.0f );	// ���_�A
	v[2].p = D3DXVECTOR3( 0.5f, -0.5f, 0.0f );	// ���_�B
	v[3].p = D3DXVECTOR3( 0.5f,  0.5f, 0.0f );	// ���_�C

	
	//���_�f�[�^�F���_�̍��W�l�̐ݒ�
    v[0].tu = 0.0f;	// ���_�@
    v[0].tv = 0.0f;	// ���_�@
    v[1].tu = 1.0f;	// ���_�A
    v[1].tv = 0.0f;	// ���_�A
    v[2].tu = 0.0f;	// ���_�B
    v[2].tv = 1.0f;	// ���_�B
    v[3].tu = 1.0f;	// ���_�C
    v[3].tv = 1.0f;	// ���_�C
  
    pMyVB->Unlock();
}
VertexObject::~VertexObject(){
	if(myTex){
		SAFE_RELEASE(pTex);
	}
}

void VertexObject::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();

	if(FAILED(rEff[0]->SetTechnique())){
		return;
	}

	// �V���h�E�}�b�v�փA�N�Z�X����Ƃ��̃I�t�Z�b�g�ݒ�
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f/rEff[1]->width;
	tmpOffset.y = 0.5f/rEff[1]->height;
	if(FAILED(rEff[0]->SetVector(&tmpOffset,"g_offset"))){
		return;
	}

	if(FAILED(rEff[0]->SetMatrix(&matWorld,"g_world"))){
		return;
	}

	D3DXMATRIX matInv;
	D3DXMatrixInverse(&matInv,NULL,&matWorld);


	if(FAILED(rEff[0]->SetMatrix(&matInv,"g_world_inv"))){
		return;
	}

	if(FAILED(rEff[0]->SetBool(&use_shadow,"g_use_shadow"))){
		return;
	}
	// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
	GetDevice()->SetStreamSource( 0, pMyVB, 0, sizeof(MY_VERTEX) );

	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	GetDevice()->SetFVF( MY_VERTEX_FVF );

	UINT numPass;

	if(FAILED(rEff[0]->pFX->Begin(&numPass, 0))){
		return;
	}

	if(pTex != 0){
		rEff[0]->pFX->BeginPass(0);
	}
	else{
		rEff[0]->pFX->BeginPass(1);
	}

	//GetDevice()->SetTexture(0,pTex);
	D3DXVECTOR4		tmpColor;
	rEff[0]->SetTexture(pTex,"g_texture");
	tmpColor.x = 1.0f;
	tmpColor.y = 1.0f;
	tmpColor.z = 1.0f;
	tmpColor.w = alpha;
	if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color0"))){
		return;
	}

	tmpColor.x = 1.0f;
	tmpColor.y = 1.0f;
	tmpColor.z = 1.0f;
	tmpColor.w = 1.0f;
	if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color1"))){
		return;
	}

	if(FAILED(rEff[0]->pFX->CommitChanges())){
		return;
	}
	// �`��
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	rEff[0]->pFX->EndPass();

	rEff[0]->pFX->End();
/*D3DMATERIAL9 material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse.r = 0.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 0.0f;
	material.Diffuse.a = 1.0f;

	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;

	material.Specular.a = 1.0f;


	material.Emissive.r = 0.3f;
	material.Emissive.g = 0.3f;
	material.Emissive.b = 0.3f;
	material.Emissive.a = 1.0f;

	GetDevice()->SetMaterial(&material);*/
}
HRESULT VertexObject::SetupTexture(char* filename){
	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFile( GetDevice(), filename,&pTex))){
			MessageBox( NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
			return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "���łɃe�N�X�`�����o�^����Ă���2D�I�u�W�F�N�g�Ƀe�N�X�`����ǂݍ������Ƃ��܂����B");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}HRESULT VertexObject::SetupTexture(char* filename,int width,int height){

	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFileEx( GetDevice(), filename,width,height,
			0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&pTex))){
				MessageBox( NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
				return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "���łɃe�N�X�`�����o�^����Ă���2D�I�u�W�F�N�g�Ƀe�N�X�`����ǂݍ������Ƃ��܂����B");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}