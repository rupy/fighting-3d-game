#include "FirstPersonPlayer.h"

FPPlayer::FPPlayer(Camera* cam)
:Player(cam),mode(MOVE_ABS)
{
}

FPPlayer::~FPPlayer(){
}

void FPPlayer::MoveAbsolutely(KeyMessage* keyMessage){
	if(keyMessage->btn & KEY_LEFT){
		cam->eyePt.x--;
	}
	if(keyMessage->btn & KEY_RIGHT){
		cam->eyePt.x++;
	}
	if(keyMessage->btn & KEY_UP){
		cam->eyePt.z--;
	}
	if(keyMessage->btn & KEY_DOWN){
		cam->eyePt.z++;
	}
	if(keyMessage->cmd & NEW_A){
	}

}

void FPPlayer::MoveRelatively(KeyMessage* keyMessage){
	D3DXVECTOR3 moveVec = cam->lookatPt - cam->eyePt;
	D3DXVec3Normalize(&moveVec,&moveVec);
	//進行方向の単位ベクトル


	if(keyMessage->btn & KEY_LEFT){
	}
	if(keyMessage->btn & KEY_RIGHT){
	}
	if(keyMessage->btn & KEY_UP){
		cam->eyePt += moveVec;
	}
	if(keyMessage->btn & KEY_DOWN){
	}
	if(keyMessage->cmd & NEW_A){
	}
}
void FPPlayer::Move(KeyMessage* keyMessage){
	switch(mode){
		case MOVE_ABS:
			MoveAbsolutely(keyMessage);
			break;
		case MOVE_REL:
			MoveRelatively(keyMessage);
			break;
		case MOVE_FLY:
			break;
		case MOVE_STOP:
			break;
		default:
			break;
	}
	cam->CameraSetting();
}