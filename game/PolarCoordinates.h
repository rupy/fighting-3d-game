#ifndef __POLAR_COORDINATES_H__
#define __POLAR_COORDINATES_H__

//�ɍ��W�n�̃r���{�[�h�N���X

#include "BillboardObject.h"

class PolarCoordinates:public BillboardObject{
public:
	bool mode;//���[�htrue�i�~�����W�j���[�hfalse�i�����W�j
	float height,	v_height;	//����
	float theta,	v_theta;	//�����ʂɑ΂���p�x
	float phi,		v_phi;		//�����ʂɑ΂���p�x�i�~�����W�n�ł͎g��Ȃ��j
	float radius,	v_radius;	//���a

	PolarCoordinates(
		vector<Effect*>& eff,
		LPDIRECT3DTEXTURE9 tex=NULL,
		bool mode		= true,
		float height	= 0,
		float radius	= 1.0f,
		float theta		= 0,
		float phi		= 0
		);
	~PolarCoordinates();

	D3DXMATRIXA16 MatrixSetting();
	D3DXMATRIXA16 BillboardMatrixSetting();
	D3DXMATRIXA16 Cylindrical();//�~�����W
	D3DXMATRIXA16 Spherical();//�����W

};
#endif	/*__POLAR_COORDINATES_H__*/