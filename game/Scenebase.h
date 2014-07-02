#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__

#include <windows.h>
#include "Light.h"
#include "Input.h"
#include "Sound.h"
#include "Effect.h"
#include <vector>
#include "SaveManager.h"
#include "RenderTarget.h"
#include "Camera.h"

class SceneBase{

public:
	bool					canClear;	//��ʂ��N���A���Ă��悢���i���[�r�[�Đ����Ȃǂ̓N���A����Ƃ��������Ȃ�j
	Light*					light;		//���C�g���Ǘ�����\����
	vector<Effect*>			eff;		//�G�t�F�N�g�f�o�C�X�i�V�F�[�_�[�t�@�C���̐���������j
	vector<RenderTarget*>	target;		//�؂�ւ������_�����O�^�[�Q�b�g
	vector<Camera*>			camera;		//�J�����i���_�̐������p�ӂ���j

	D3DPRESENT_PARAMETERS* pD3DPPP;		//Direct3D�p�����[�^�[
protected:
	int sceneMode;						//�V�[�����[�h

	//=============================
	//�f�o�C�X�Ȃ�
	//=============================
	LPD3DXSPRITE			pSprite;	//�X�v���C�g�f�o�C�X
	LPDIRECT3DDEVICE9		pDevice;	//DirectX�f�o�C�X
	Sound*					pSound;		//�T�E���h�f�o�C�X
	SaveManager*			pSM;		//�Z�[�u�f�[�^

public:
	SceneBase(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm);
	virtual ~SceneBase();

	//����������
	virtual HRESULT Initialize()=0;

	//�Q�[���̎��s
	virtual SceneBase* Move(KeyMessage* keyMessage)=0;
	//�`��
	virtual void Draw()=0;
	//2D�`��̂��߂ɃN���A����i�r���{�[�h�܂ށj
	void ClearFor2DDraw();
	//2D�̂��߂Ƀ����_�[�X�e�[�g��ݒ肷��i�r���{�[�h�܂܂Ȃ��j
	void SetRenderStateFor2D();
};


#endif	/*__SCENEBASE_H__*/