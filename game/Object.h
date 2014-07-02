#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <d3dx9.h>
#include "MyFunc.h"
#include "ResourceBase.h"
class Object:public ResourceBase
{
public:
	//位置・回転・スケール
	D3DXVECTOR3	Pos;
	float		Yaw;
	float		Pitch;
	float		Roll;
	D3DXVECTOR3	Scale;

	//向く方向
	D3DXVECTOR3 lookatPt;
	D3DXVECTOR3 upVec;

	Object* parentObj;
	bool	validParent;

	//物理運動
	D3DXVECTOR3 Velo;//速度
	D3DXVECTOR3 Inertia;//慣性の速度
	D3DXVECTOR3 Accel;//加速度

	bool top,land,wall;//着地と天井、壁チェック
	bool use_shadow;

protected:
public:

	Object(D3DXVECTOR3 pos=D3DXVECTOR3(0.0f,0.0f,0.0f), float Y=0.0f,float R=0.0f,float P=0.0f,D3DXVECTOR3 scale = D3DXVECTOR3(1.0f,1.0f,1.0f));
	//Object(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos=D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3 lookat=D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR3 upvec=D3DXVECTOR3(0.0f,0.0f,0.0f));

	//マトリックスの計算
	virtual D3DXMATRIXA16 MatrixSetting();
	virtual D3DXMATRIXA16 BillboardMatrixSetting();
	//自分を描画
	virtual void Appear()=0;
	virtual void Shadow();

	void Move();
	void Gravity(float gravity_weight);
	void ResetVelocity();

	void SetParent(Object* parent);

};

#endif	/*__OBJECT_H__*/