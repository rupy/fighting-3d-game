#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <d3dx9.h>
#include "Object.h"
#include "Effect.h"
#include "ResourceBase.h"


#ifndef PI_HALF
	#define	PI_HALF			( D3DX_PI / 2.0f )			// 90度
#endif

#ifndef DEG_TO_RAD
	#define	DEG_TO_RAD		( D3DX_PI * 2.0f / 360.0f)
#endif

#ifndef PI_1FOURTH
	#define	PI_1FOURTH		( D3DX_PI / 4.0f )			// 45度
#endif

#define ZOOM_MAX	20.0f
#define ZOOM_MIN	10.0f

class Camera:public ResourceBase
{
	vector<Effect*>&		rEff;

	float		distance;		//カメラの距離
	float		speed;			//カメラのスピード
	float		rSpeed;			//回転速度
public:
	D3DXVECTOR3	eyePt;			//カメラ位置
	D3DXVECTOR3	lookatPt;		//注視点
	D3DXVECTOR3	upVec;			//カメラの上方方向


//	Camera();
	Camera(vector<Effect*>& eff, D3DXVECTOR3 eye=D3DXVECTOR3(0,-1.0f,0), D3DXVECTOR3 look=D3DXVECTOR3(0,0,0), D3DXVECTOR3 up=D3DXVECTOR3(0,1.0f,0));
	~Camera();

	void SetDistance(float distance);
	void ZoomIn(float minus_dist);
	void ZoomOut(float plus_dist);

	void SetSpeed(float speed);
	void SetRSpeed(float rSpeed);

	void SeeObject(const Object* obj);
	void RunAfterObject(const Object* obj);
	void TurnRoundBehindObject(const Object* obj);
	void TurnRoundObjectR(const Object* obj);
	void TurnRoundObjectL(const Object* obj);
	void StayOverObject(const Object* obj,float height);

	void CameraSetting();
};

#endif	/*__CAMERA_H__*/