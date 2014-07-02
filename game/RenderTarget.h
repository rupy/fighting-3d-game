#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include <d3dx9.h>
#include "ResourceBase.h"

class RenderTarget:public ResourceBase{
	
	// �o�b�N�o�b�t�@�[�̑ޔ�p
	D3DVIEWPORT9			vpOrig;
	LPDIRECT3DSURFACE9      pBackbuffer;	// �o�b�N�o�b�t�@�[
    LPDIRECT3DSURFACE9		pBackZ;		// �o�b�N�o�b�t�@�[�p�̐[�x�o�b�t�@�[

	//�����_�����O�^�[�Q�b�g�i�I���W�i���T�C�Y�j
	int width;
	int height;
	LPDIRECT3DTEXTURE9      pTexture0;		// �e�N�X�`��
	LPDIRECT3DSURFACE9      pTexSurface0;	// �e�N�X�`������擾����T�[�t�F�[�X
    LPDIRECT3DSURFACE9		pTexZ0;		// �e�N�X�`���p�̐[�x�o�b�t�@�[

public:
	RenderTarget(LPDIRECT3DDEVICE9 device);
	~RenderTarget();


	HRESULT InitSurface(int width,int height);
	void ReleaseSurface();
	HRESULT ResetSurface();
	HRESULT BeginToggleBackBuffer();
	HRESULT EndToggleBackBuffer();

	void SetOffset();

	LPDIRECT3DTEXTURE9 GetTexture();
	int GetTargetWidth();
	int GetTargetHeight();
};


#endif	/*__RENDER_TARGET_H__*/