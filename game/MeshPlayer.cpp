#include "MeshPlayer.h"

MeshPlayer::MeshPlayer(Camera* cam,MeshObject* obj)
:Player(cam),obj(obj),mode(MOVE_REL)
{
}

MeshPlayer::~MeshPlayer(){
}

void MeshPlayer::MoveAbsolutely(KeyMessage* keyMessage){
			if(keyMessage->btn & KEY_LEFT){
				obj->Pos.x--;
			}
			if(keyMessage->btn & KEY_RIGHT){
				obj->Pos.x++;
			}
			if(keyMessage->btn & KEY_UP){
				obj->Pos.z++;
			}
			if(keyMessage->btn & KEY_DOWN){
				obj->Pos.z--;
			}

}

void MeshPlayer::MoveRelatively(KeyMessage* keyMessage){

	D3DXVECTOR3 stickVec(0.0f,0.0f,0.0f);

	D3DXVECTOR3 camVec = cam->lookatPt - cam->eyePt;
	camVec.y = 0.0f;

	float camRot = atan2f(camVec.x,camVec.z);

	float rot = 0;
	//�Е��̒l�̐�Βl���������l�𒴂��Ă����ꍇ�̓X�e�B�b�N���͂��������ƔF������
	if(fabs((double)keyMessage->rightleft)>JS_DEAD
		||fabs((double)keyMessage->updown)>JS_DEAD)
	{
		stickVec = D3DXVECTOR3(keyMessage->rightleft,0.0f,-keyMessage->updown);
		
		D3DXVec3Normalize(&stickVec,&stickVec);
		rot = atan2f(-stickVec.x,-stickVec.z);

			//�������l(�R�T�O)�`�T�O�O
		if(fabs((double)keyMessage->rightleft)>800
			||fabs((double)keyMessage->updown)>800)
		{
			stickVec *= MOVE_SPEED_FAST;
		}
		else if(fabs((double)keyMessage->rightleft)>600
			||fabs((double)keyMessage->updown)>600)
		{
			stickVec *= MOVE_SPEED_NORMAL;
		}
		else{
			stickVec *= MOVE_SPEED_SLOW;
		}
		D3DXMATRIX around;
		D3DXMatrixRotationY(&around,camRot);
		D3DXVec3TransformCoord(&stickVec,&stickVec,&around);

	
		rot += camRot;
		obj->Yaw= rot?rot:obj->Yaw;//�p�x��0�������玩���̍��̊p�x��ێ�����
		obj->Pos += stickVec;

	}else{
	}


	cam->RunAfterObject(obj);

	if(keyMessage->zrightleft > 500)
		cam->TurnRoundObjectR(obj);
	else if(keyMessage->zrightleft < -500)
		cam->TurnRoundObjectL(obj);

	cam->CameraSetting();

	difference = stickVec;

}
void MeshPlayer::FlyInTheSky(KeyMessage* keyMessage){}
void MeshPlayer::Gravity(){
	obj->Velo.y -= 0.3f;
}
void MeshPlayer::Move(KeyMessage* keyMessage){
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
}