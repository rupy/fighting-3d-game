#ifndef __ANIM_PLAYER_H__
#define __ANIM_PLAYER_H__

#include "Player.h"
#include "AnimObject.h"

#define JUMP_SPEED 1.0f
class AnimPlayer:public Player{
public:
	AnimObject*	obj;
	MoveMode	mode;

	//差分（移動量）
	D3DXVECTOR3 difference;

	//ジャンプ
	float	startJump;
	float	jumpSpeed;
	float	jumpAccel;
	bool	nowJump;

	AnimPlayer(Camera* cam,AnimObject* obj);
	~AnimPlayer();

	void MoveAbsolutely(KeyMessage* keyMessage);
	void MoveRelatively(KeyMessage* keyMessage);
	void FlyInTheSky(KeyMessage* keyMessage);

	void GetStickVec(KeyMessage* keyMessage,D3DXVECTOR3* stick,float* cam_rot);
	void Relativize(D3DXVECTOR3* stick_rel,const D3DXVECTOR3* stick_abs,float* rot_rel,const float* rot_abs);
	void Gravity();

	void Move(KeyMessage* keyMessage);
};

#endif	/*__MESH_PLAYER_H__*/