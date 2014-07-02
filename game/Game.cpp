#include "Game.h"
#include "Common.h"

LPDIRECT3DDEVICE9 ResourceBase::pDevice;
//=============================================================
// �R���X�g���N�^
//=============================================================
GameBase::GameBase(LPDIRECT3DDEVICE9		device,
				   D3DPRESENT_PARAMETERS*	param,
				   LPD3DXSPRITE				sprite,
				   Input*					input,
				   Sound*					sound)
:pDevice(device),pD3DPP(param),pSprite(sprite),pInput(input),pSound(sound),
scene(NULL)
{
	ResourceBase::SetDevice(device);

	SM.InitState();
	scene = new StartScene(device,sprite,pSound,&SM);//�͂��߂̓V�[���P
	if(FAILED(scene->Initialize())){		
		PostQuitMessage(0);
	}

}
//=============================================================
// �f�X�g���N�^
//=============================================================
GameBase::~GameBase(){

	SAFE_DELETE(scene);
}
//=============================================================
// �Q�[���̎��s
//=============================================================
int GameBase::Run(){

	KeyMessage keyMessage = pInput->GetKeybordState();
	pInput->GetJoystickState(&keyMessage,0);
	pInput->GetCommand(&keyMessage);

	SceneBase* next=NULL;

	next = scene->Move(&keyMessage);
	if(next!=NULL){
		//�V�����V�[�����󂯎������V�[����؂�ւ��ď�����
		SAFE_DELETE(scene);
		scene = next;
		scene->Initialize();
		scene->Move(&keyMessage);//�V�����V�[���ł̕`��
	}

	return 0;
}

//=============================================================
// �o�b�N�o�b�t�@�֏�������
//=============================================================
HRESULT GameBase::Draw(){


	if(scene->canClear){
		RecetRenderState();//�����_�[�X�e�[�g�̃��Z�b�g

		// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̏������ƃX�e���V���o�b�t�@�̍폜
		if(FAILED(pDevice->Clear(0,NULL, 				// ����������̈�͑S��
			D3DCLEAR_TARGET |					// �o�b�N�o�b�t�@���w��
			D3DCLEAR_ZBUFFER, 					// �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
			D3DXCOLOR(0.7f,0.9f,0.9f, 1.0f), 	// ����������F
			1.0f, 								// ����������[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
			0))) {								// ����������X�e���V���o�b�t�@�̒l
				return(E_FAIL);
		}

		// �`��J�n�錾
		if(SUCCEEDED(pDevice->BeginScene())) {
			// ���f���Ȃǂ̕`��
			scene->Draw();
			// �`��I���錾
			pDevice->EndScene();
		}
	}//else{
	//			scene->Draw();
	//}


	return(S_OK);
}

//=============================================================
// ��ʂ̕\��
//=============================================================
HRESULT GameBase::Display()
{
	if(scene->canClear){
		// �`�挋�ʂ̓]��
		if(FAILED(pDevice->Present( 0, 0, 0, 0 ))) {
			// �f�o�C�X�������畜�A�����݂�
			if(FAILED(DeviceRecet())){
				return E_FAIL;
			}
		}
	}

	return S_OK;
}
//=============================================================
// ��{�I�ȃ����_�[�X�e�[�g�̐ݒ�
//=============================================================
void GameBase::RecetRenderState(){
//	// ���ʕ`�惂�[�h�̎w��
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	// �y��r���s�Ȃ�
//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
//	// �f�B�U�����O���s�Ȃ��i���i���`��j
//	pDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
//	// �m�[���C�e�B���O���[�h 	
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
//	// ���� 	
//	pDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080);
//
//	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1,	D3DTA_TEXTURE );	// �e�N�X�`���̐F���g�p
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2,	D3DTA_DIFFUSE );	// ���_�̐F���g�p
	pDevice->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_MODULATE );	// ��Z����
//
	pDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	pDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
	pDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

//	pDevice->LightEnable(0,FALSE);

}
//=============================================================
// �f�o�C�X�̃��Z�b�g
//=============================================================
HRESULT GameBase::DeviceRecet(){

	HRESULT hr;

	//=============================
	//���Z�b�g
	//=============================
	//�X�v���C�g
	pSprite->OnLostDevice();

	//�G�t�F�N�g
	for(size_t i=0;i<scene->eff.size();++i){
		scene->eff[i]->pFX->OnLostDevice();
	}

	for(size_t i=0;i<scene->target.size();++i){
		scene->target[i]->ReleaseSurface();
	}

	//=============================
	//�f�o�C�X���X�g�̃`�F�b�N
	//=============================
	if(FAILED(hr = pDevice->TestCooperativeLevel())){
		if(hr == D3DERR_DEVICELOST){
			Sleep(100);								// 0.1�b�҂�
			return S_OK;
		}
		if(hr != D3DERR_DEVICENOTRESET){
			return E_FAIL;
		}

		if(FAILED(hr = pDevice->Reset(pD3DPP))){
			if(hr == D3DERR_DEVICELOST){
				return S_OK;
			}
			return E_FAIL;
		}
	}

	//=============================
	//���Z�b�g�ƕ���
	//=============================
	//�f�o�C�X
	pDevice->Reset(pD3DPP);
	//�X�v���C�g
	pSprite->OnResetDevice();
	//�G�t�F�N�g
	for(size_t i=0;i<scene->eff.size();++i){
		scene->eff[i]->pFX->OnResetDevice();
	}
	for(size_t i=0;i<scene->target.size();++i){
		scene->target[i]->ResetSurface();
	}
	//RecetRenderState();

	return S_OK;
}