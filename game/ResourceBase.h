#ifndef __RESOURCE_BASE_H__
#define __RESOURCE_BASE_H__

//�v���O�����Ŏg�����\�[�X�͂��ׂăf�o�C�X���g���̂ŁA���\�[�X���ׂĂɂ��̃N���X���p�������Ďg�p����
//���̃N���X���g���ꍇ�ɂ͐ÓI�����o��K�����������Ă�������

#include <d3dx9.h>


class ResourceBase{
private:
	static LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X

public:
	//�f�o�C�X���󂯎��A�N�Z�T���\�b�h
	inline LPDIRECT3DDEVICE9 GetDevice(){
		return pDevice;
	}
	static inline void SetDevice(LPDIRECT3DDEVICE9 device){
		pDevice = device;
	}
};

#endif	/*__RESOURCE_BASE_H__*/