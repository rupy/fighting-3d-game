#include "CameraOperator.h"

CameraOperator::CameraOperator(Camera *cam,Object* target)
:cam(cam),target(target)
{
}
void CameraOperator::Move(KeyMessage* keyMessage){
	cam->RunAfterObject(target);

	if(keyMessage->zrightleft > 500 || keyMessage->btn & KEY_R)
		cam->TurnRoundObjectR(target);
	else if(keyMessage->zrightleft < -500 || keyMessage->btn & KEY_L)
		cam->TurnRoundObjectL(target);

	if(keyMessage->btn & KEY_S){
		cam->TurnRoundBehindObject(target);
	}

	cam->StayOverObject(target,5.0f);
	if(keyMessage->zupdown > 500)
		cam->ZoomOut(0.1f);
	else if(keyMessage->zupdown < -500)
		cam->ZoomIn(0.1f);

	cam->CameraSetting();
}
void CameraOperator::SetTarget(Object* target){
	this->target = target;
}