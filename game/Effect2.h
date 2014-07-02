#ifndef __EFFECT_H__
#define __EFFECT_H__

#include <d3dx9.h>
#include <tchar.h>
#include <map>
#include <string>
#include <vector>
#include "common.h"

using namespace std;

struct EffectInfo{
	string	fileName;
	string	techName;
	vector<string>	valueName;
};

class Effect{
public:
	LPD3DXEFFECT	pFX;
	LPDIRECT3DDEVICE9 pDevice;

	EffectInfo info;

	// �e�N�j�b�N�ւ̃n���h��
	D3DXHANDLE              hTech;

	// �V�F�[�_�v���O�����̃O���[�o���ϐ��ւ̃n���h��
	map<string,D3DXHANDLE>	hValue;

	// �o�b�N�o�b�t�@�[�̑ޔ�p
	D3DVIEWPORT9 viewport1;
	LPDIRECT3DSURFACE9      pBackbuffer;	// �o�b�N�o�b�t�@�[
    LPDIRECT3DSURFACE9		pBackZ;		// �o�b�N�o�b�t�@�[�p�̐[�x�o�b�t�@�[

	//�����_�����O�^�[�Q�b�g�i�I���W�i���T�C�Y�j
	LPDIRECT3DTEXTURE9      pTexture0;		// �e�N�X�`��
	LPDIRECT3DSURFACE9      pTexSurface0;	// �e�N�X�`������擾����T�[�t�F�[�X
    LPDIRECT3DSURFACE9		pTexZ0;		// �e�N�X�`���p�̐[�x�o�b�t�@�[


	Effect();
	~Effect();
	HRESULT InitEffect(LPDIRECT3DDEVICE9 device,const EffectInfo* info);
	LPD3DXEFFECT GetEffect();
	HRESULT Reset();

	HRESULT SetMatrix(const D3DXMATRIX* mat ,char* handle_name);
	HRESULT SetVector(const D3DXVECTOR4* vec ,char* handle_name);
	HRESULT SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name);
	HRESULT SetTechnique();

	HRESULT InitSurface();
	HRESULT BeginToggleBackBuffer();
	HRESULT EndToggleBackBuffer();

private:
	HRESULT EffectCompile();
	HRESULT HandleSetting();

};


#endif	/*__EFFECT_H__*/