#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "Player.h"
#include "AnimObject.h"

#define JUMP_SPEED 1.0f

#define HUMAN_SPEED		0.15f
#define DECAY_RATE		0.8f
#define SWIM_SPEED		0.2f

class Human:public Player{
public:
	enum HumanMode{
		MOVE_ABS,//カメラと無関係に絶対移動
		MOVE_REL,//カメラとの相対移動
		MOVE_FLY,//飛行
		MOVE_SWIM,//泳ぐ
	};

	AnimObject*	obj;
	HumanMode	mode;
	//移動
	D3DXVECTOR3 moveVelo;

	//ジャンプ
	float	startJump;
	float	jumpSpeed;
	float	jumpAccel;
	bool	nowJump;

	Human(Camera* cam,AnimObject* obj);
	~Human();

	void MoveAbsolutely(KeyMessage* keyMessage);
	void MoveRelatively(KeyMessage* keyMessage);
	void FlyInTheSky(KeyMessage* keyMessage);
	void Swim(KeyMessage* keyMessage);

	void GetStickVec(KeyMessage* keyMessage,D3DXVECTOR3* stick,float* cam_rot);
	void Relativize(D3DXVECTOR3* stick_rel,const D3DXVECTOR3* stick_abs,float* rot_rel,const float* rot_abs);
	void Jump(KeyMessage* keyMessage);
	void Animation();
	void Gravity();
	void Surface();

	void Move(KeyMessage* keyMessage);
};

#endif	/*__HUMAN_H__*/