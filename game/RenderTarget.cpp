#include "RenderTarget.h"
#include "Common.h"
#include <dxerr.h>
#include "Error.h"

RenderTarget::RenderTarget(LPDIRECT3DDEVICE9 device)
:width(0),height(0),
pBackbuffer(NULL),pBackZ(NULL),
pTexture0(NULL),pTexSurface0(NULL),pTexZ0(NULL)
{
}
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(pBackbuffer);
	SAFE_RELEASE(pBackZ);
	ReleaseSurface();
}

HRESULT RenderTarget::InitSurface(int width,int height){

	this->width = width;
	this->height = height;
	HRESULT hr;
	// �V���h�E�}�b�v�Ƃ��Ďg�p����e�N�X�`���̍쐬
	if( FAILED(GetDevice()->CreateTexture(
		width, height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A32B32G32R32F,	// �t���[�e�B���O�o�b�t�@���쐬		
		D3DPOOL_DEFAULT,
		&pTexture0,
		NULL))) {
			if( FAILED(hr = GetDevice()->CreateTexture(
				width, height,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_R16F,
				D3DPOOL_DEFAULT,
				&pTexture0,
				NULL))) {
					if( FAILED(hr = GetDevice()->CreateTexture(
						width, height,
						1,
						D3DUSAGE_RENDERTARGET,
						D3DFMT_G16R16F,
						D3DPOOL_DEFAULT,
						&pTexture0,
						NULL))) {
							if( FAILED(hr = GetDevice()->CreateTexture(
								width, height,
								1,
								D3DUSAGE_RENDERTARGET,
								D3DFMT_G16R16,
								D3DPOOL_DEFAULT,
								&pTexture0,
								NULL))) {
									if( FAILED(hr = GetDevice()->CreateTexture(
										width, height,
										1,	
										D3DUSAGE_RENDERTARGET,
										D3DFMT_A16B16G16R16,
										D3DPOOL_DEFAULT,
										&pTexture0,
										NULL))) {
											return E_FAIL;
									}
							}
					}
			}
	}

	// �e�N�X�`�����烌���_�����O�^�[�Q�b�g�ɂ���T�[�t�F�[�X�̎擾
	if( FAILED(pTexture0->GetSurfaceLevel(0, &pTexSurface0))){
		return E_FAIL;
	}

	// �e�N�X�`���ւ̃����_�����O�Ɏg���[�x�o�b�t�@�[�̍쐬
	if (FAILED(GetDevice()->CreateDepthStencilSurface(
						width, height,		
						D3DFMT_D24X8,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&pTexZ0,
						NULL))){
		DXTRACE_ERR("<�����_�[�^�[�Q�b�g�̂��߂̐[�x�o�b�t�@�쐬>",hr);
		return E_FAIL;
	}
	return S_OK;
}
HRESULT RenderTarget::ResetSurface(){
	if(width&&height){
		if(FAILED(InitSurface(width,height))){
			return E_FAIL;
		}
	}
	return S_OK;
}
void RenderTarget::ReleaseSurface(){
	SAFE_RELEASE(pTexture0);
	SAFE_RELEASE(pTexSurface0);
	SAFE_RELEASE(pTexZ0);
}
HRESULT RenderTarget::BeginToggleBackBuffer(){

	//========================================
	// �����F�o�b�N�o�b�t�@�[�̑ޔ�
	//========================================

	// �r���[�|�[�g���̕ێ�
	GetDevice()->GetViewport(&vpOrig);

	// �o�b�N�o�b�t�@�[�ւ̃C���^�[�t�F�[�X�̕ێ�
	GetDevice()->GetRenderTarget(0, &pBackbuffer);

	// �[�x�o�b�t�@�[�̃C���^�[�t�F�[�X�̕ێ�
    GetDevice()->GetDepthStencilSurface( &pBackZ);

	//========================================
	//�p�X�P�F�V�[���̐[�x������`��
	//========================================

	// �e�N�X�`���������_�����O�^�[�Q�b�g�ɐݒ�
	GetDevice()->SetRenderTarget(0, pTexSurface0);

	// �e�N�X�`���p�̐[�x�o�b�t�@��ݒ�
	GetDevice()->SetDepthStencilSurface(pTexZ0);

	// �r���[�|�[�g���e�N�X�`���摜�̃T�C�Y�ɐݒ�
	D3DVIEWPORT9 viewport2 = { 0,0,width,height, 0.0f, 1.0f};
	GetDevice()->SetViewport(&viewport2);

	if(FAILED(GetDevice()->Clear(0,NULL, 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
		1.0f, 0))) {
			return E_FAIL;
	}
	return S_OK;

}
HRESULT RenderTarget::EndToggleBackBuffer(){
	

	//==========================================
	//�^�[�Q�b�g��߂�
	//==========================================

	// �ۑ����Ă������o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�ɐݒ�
	GetDevice()->SetRenderTarget(0, pBackbuffer);

	// �ۑ����Ă������o�b�N�o�b�t�@�̐[�x�o�b�t�@��ݒ�
	GetDevice()->SetDepthStencilSurface(pBackZ);

	// �ۑ����Ă������r���[�|�[�g�����ɖ߂�
	GetDevice()->SetViewport(&vpOrig);

	SAFE_RELEASE(pBackbuffer);
	SAFE_RELEASE(pBackZ);

	return S_OK;

}

void SetOffset(){
}

LPDIRECT3DTEXTURE9 RenderTarget::GetTexture(){
	return pTexture0;
}
int RenderTarget::GetTargetWidth(){
	return width;
}
int RenderTarget::GetTargetHeight(){
	return height;
}