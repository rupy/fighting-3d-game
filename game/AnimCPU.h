#ifndef __ANIM_CPU_H__
#define __ANIM_CPU_H__

#include "AnimObject.h"
#include "Operator.h"


class AnimCPU:public Operator{
public:
	enum CPUMode{
		aaaa,
		bbb,
		cc,
	};

	int HP;
	float jumpSpeed;
	float jumptime;

	AnimObject*	obj;
	CPUMode		mode;
	Object*		targetObj;
	float		speed;
	D3DXVECTOR3		difference;
	bool		nowJump;
	float		territory;

	AnimCPU(AnimObject* obj);

	void SetTargetObject(Object* obj);
	void Move();
	void Gravity();

	void RunAfterTarget();
	void ComeAfterTarget();
	bool CheckTerritory();

};

#endif	/*__ANIM_CPU_H__*/