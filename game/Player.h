#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Input.h"
#include "Camera.h"
#include "Operator.h"

#define MOVE_SPEED_FAST		0.4f
#define MOVE_SPEED_NORMAL	0.2f
#define MOVE_SPEED_SLOW		0.05f

enum MoveMode{
	MOVE_ABS,//�J�����Ɩ��֌W�ɐ�Έړ�
	MOVE_REL,//�J�����Ƃ̑��Έړ�
	MOVE_FLY,//��s
	MOVE_STOP,//��~
};

class Player:public Operator{
public:

	Camera*	cam;
	
	Player(Camera* cam)
		:cam(cam){}

	virtual void MoveRelatively(KeyMessage* keyMessage)=0;
	virtual void MoveAbsolutely(KeyMessage* keyMessage)=0;
//	virtual void FlyInTheSky(KeyMessage* keyMessage)=0;

	virtual void Move(KeyMessage* keyMessage)=0;
};

#endif	/*__PLAYER_H__*/