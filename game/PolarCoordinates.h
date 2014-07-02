#ifndef __POLAR_COORDINATES_H__
#define __POLAR_COORDINATES_H__

//極座標系のビルボードクラス

#include "BillboardObject.h"

class PolarCoordinates:public BillboardObject{
public:
	bool mode;//モードtrue（円筒座標）モードfalse（球座標）
	float height,	v_height;	//高さ
	float theta,	v_theta;	//水平面に対する角度
	float phi,		v_phi;		//垂直面に対する角度（円筒座標系では使わない）
	float radius,	v_radius;	//半径

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
	D3DXMATRIXA16 Cylindrical();//円筒座標
	D3DXMATRIXA16 Spherical();//球座標

};
#endif	/*__POLAR_COORDINATES_H__*/