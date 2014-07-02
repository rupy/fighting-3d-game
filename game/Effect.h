#ifndef __EFFECT_H__
#define __EFFECT_H__

#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>
#include "Common.h"
#include "ResourceBase.h"

using namespace std;

struct EffectInfo{
	string	fileName;
	string	techName;
	vector<string>	valueName;
};

class Effect:public ResourceBase{
public:
	LPD3DXEFFECT	pFX;

	EffectInfo info;

	// �e�N�j�b�N�ւ̃n���h��
	D3DXHANDLE              hTech;

	// �V�F�[�_�v���O�����̃O���[�o���ϐ��ւ̃n���h��
	map<string,D3DXHANDLE>	hValue;

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


	Effect();
	~Effect();
	HRESULT InitEffect(const EffectInfo* info);
	LPD3DXEFFECT GetEffect();
	HRESULT Reset();

	HRESULT SetMatrix(const D3DXMATRIX* mat ,char* handle_name);
	HRESULT SetVector(const D3DXVECTOR4* vec ,char* handle_name);
	HRESULT SetBool(const bool* value ,char* handle_name);
	HRESULT SetFloat(const float* value ,char* handle_name);
	HRESULT SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name);
	HRESULT SetTechnique();


private:
	HRESULT EffectCompile();
	HRESULT HandleSetting();

};


#endif	/*__EFFECT_H__*/