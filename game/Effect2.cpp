#include "effect2.h"

Effect::Effect()
:pFX(NULL)
{

}
Effect::~Effect(){
	SAFE_RELEASE(pFX);
}
HRESULT Effect::InitEffect(LPDIRECT3DDEVICE9 device,const EffectInfo* info){


	this->info = *info;
	pDevice = device;
	//�G�t�F�N�g�t�@�C���̃R���p�C��
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}

	//�n���h���̎擾
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}


	return S_OK;
}

HRESULT Effect::EffectCompile(){

	LPD3DXBUFFER err=NULL;
	if(FAILED(D3DXCreateEffectFromFile(pDevice,info.fileName.c_str(),NULL,NULL,D3DXSHADER_DEBUG,NULL,&pFX,&err))){
		// �R���p�C���G���[�̏ꍇ�̓G���[��\�����ďI��
		if (!pFX) {
			MessageBox(
				NULL, 
				err?(LPCTSTR)err->GetBufferPointer():NULL, 
				"�G�t�F�N�g�t�@�C���̓Ǎ��G���[", MB_OK);
		}
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Effect::HandleSetting(){

	// �V�F�[�_�v���O�����փe�N�j�b�N�ւ̃n���h���̎擾
	hTech = pFX->GetTechniqueByName(info.techName.c_str());

	for(size_t i=0; i < info.valueName.size(); ++i){
		hValue.insert( make_pair(info.valueName[i].c_str() ,pFX->GetParameterByName(0,info.valueName[i].c_str() )));
	}

	//������ƃn���h�����擾�ł��Ă��邩�`�F�b�N
	map<string,D3DXHANDLE>::iterator p;
	for(p = hValue.begin(); p != hValue.end();++p){
		if(p->second == NULL){
			return E_FAIL;
		}
	}
	return S_OK;
}
HRESULT Effect::Reset(){

	hValue.clear();

	//�G�t�F�N�g�t�@�C���̍ăR���p�C��
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}
	//�n���h���̍Ď擾
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}

	return S_OK;
}

LPD3DXEFFECT Effect::GetEffect(){
	return pFX;
}

HRESULT Effect::SetMatrix(const D3DXMATRIX* mat, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetMatrix( p->second , mat))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetVector(const D3DXVECTOR4* vec, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetVector( p->second , vec))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetTexture(p->second, tex))){
		return E_FAIL;
	}
	return S_OK;
		
}
HRESULT Effect::SetTechnique(){

	if(FAILED(pFX->SetTechnique(hTech))){
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Effect::InitSurface(){

	// �V���h�E�}�b�v�Ƃ��Ďg�p����e�N�X�`���̍쐬
	if( FAILED(pDevice->CreateTexture(
						640, 480,			
						1,	
         				D3DUSAGE_RENDERTARGET,
						D3DFMT_A8R8G8B8,	// �t���[�e�B���O�o�b�t�@���쐬
         				D3DPOOL_DEFAULT,
         				&pTexture0,
						NULL))) {	
		return E_FAIL;
	}

	// �e�N�X�`�����烌���_�����O�^�[�Q�b�g�ɂ���T�[�t�F�[�X�̎擾
	if( FAILED(pTexture0->GetSurfaceLevel(0, &pTexSurface0))){
		return E_FAIL;
	}

	// �e�N�X�`���ւ̃����_�����O�Ɏg���[�x�o�b�t�@�[�̍쐬
	if (FAILED(pDevice->CreateDepthStencilSurface(
						640, 480,		
						D3DFMT_D24X8,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&pTexZ0,
						NULL))){
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Effect::BeginToggleBackBuffer(){

	//========================================
	// �����F�o�b�N�o�b�t�@�[�̑ޔ�
	//========================================

	// �r���[�|�[�g���̕ێ�
	pDevice->GetViewport(&viewport1);

	// �o�b�N�o�b�t�@�[�ւ̃C���^�[�t�F�[�X�̕ێ�
	pDevice->GetRenderTarget(0, &pBackbuffer);

	// �[�x�o�b�t�@�[�̃C���^�[�t�F�[�X�̕ێ�
    pDevice->GetDepthStencilSurface( &pBackZ);

	//========================================
	//�p�X�P�F�V�[���̐[�x������`��
	//========================================

	// �e�N�X�`���������_�����O�^�[�Q�b�g�ɐݒ�
	pDevice->SetRenderTarget(0, pTexSurface0);

	// �e�N�X�`���p�̐[�x�o�b�t�@��ݒ�
	pDevice->SetDepthStencilSurface(pTexZ0);

	// �����_�����O�^�[�Q�b�g��������
	if(FAILED(pDevice->Clear(0,NULL, 
								D3DCLEAR_ZBUFFER, 
								D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
								1.0f, 0))) {
		return E_FAIL;
	}


	float light_x, light_y, light_z;

	light_x = 0.5f;
	light_y = 1.0f; 
	light_z = -2.0f;


	// �r���[�|�[�g���e�N�X�`���摜�̃T�C�Y�ɐݒ�
	D3DVIEWPORT9 viewport2 = { 0,0,640,480, 0.0f, 1.0f};
	pDevice->SetViewport(&viewport2);

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)viewport2.Width / (float)viewport2.Height;


	D3DXMATRIXA16 proj;
	D3DXMATRIXA16 view;
	// �v���W�F�N�V�����}�g���b�N�X�̍쐬�Ɛݒ�
	D3DXMatrixIdentity(&proj);
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45.0f), aspect, 1.0f, 100.0f);

	// ���W�ϊ��i�������J�����ƌ����Ă�j
	D3DXMatrixIdentity(&view);
	D3DXMatrixLookAtLH(&view, 
						&D3DXVECTOR3(light_x, light_y, light_z), 
						&D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 
						&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	
	// �e�N�j�b�N�̐ݒ�i�V�F�[�_�v���O�����̐ݒ�j
	if(FAILED(SetTechnique())){
		return E_FAIL;
	}

	// �V�F�[�_�[�̃O���[�o���ϐ��̒l�̐ݒ�
	if(FAILED(SetMatrix(&(view*proj),"g_vlp"))){
		return E_FAIL;
	}
	pFX->CommitChanges();

	// �V�F�[�_�v���O�����̊J�n�錾
	pFX->Begin(0, 0);
	pFX->BeginPass(0);
	// �V�[����`�悷��

	return S_OK;

}
HRESULT Effect::EndToggleBackBuffer(){

	pFX->EndPass();
	pFX->End();	

	//==========================================
	//�^�[�Q�b�g��߂�
	//==========================================

	// �ۑ����Ă������o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�ɐݒ�
	pDevice->SetRenderTarget(0, pBackbuffer);

	// �ۑ����Ă������o�b�N�o�b�t�@�̐[�x�o�b�t�@��ݒ�
	pDevice->SetDepthStencilSurface(pBackZ);

	// �ۑ����Ă������r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&viewport1);


	return S_OK;

}