#ifndef __CAMERA_OPERATOR_H__
#define __CAMERA_OPERATOR_H__

#include "Operator.h"
#include "Camera.h"
#include "Input.h"

class CameraOperator{
	Camera* cam;
	Object*	target;
public:
	CameraOperator(Camera* cam,Object* target);
	void Move(KeyMessage* keyMessage);

	void SetTarget(Object* target);

};

#endif	/*__CAMERA_OPERATOR_H__*/