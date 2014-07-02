#include "AnimPlayer.h"

AnimPlayer::AnimPlayer(Camera* cam,AnimObject* obj)
:Player(cam),obj(obj),mode(MOVE_REL),nowJump(false),jumpSpeed(0),jumpAccel(-0.05f),difference(0,0,0)
{
}

AnimPlayer::~AnimPlayer(){
}

void AnimPlayer::GetStickVec(KeyMessage* keyMessage,D3DXVECTOR3* stick,float* cam_rot){
	D3DXVECTOR3 stickVec(0.0f,0.0f,0.0f);
	float rot = 0;

	//�Е��̒l�̐�Βl���������l�𒴂��Ă����ꍇ�̓X�e�B�b�N���͂��������ƔF������
	if(	(fabs((double)keyMessage->rightleft)>JS_DEAD
		|| fabs((double)keyMessage->updown)>JS_DEAD)
		&&obj->GetAnimChange()
		)
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
	}
	*stick = stickVec;
	*cam_rot = rot;

}
void AnimPlayer::MoveAbsolutely(KeyMessage* keyMessage){
			if(keyMessage->btn & KEY_LEFT){
				obj->Pos.x--;
			}
			if(keyMessage->btn & KEY_RIGHT){
				obj->Pos.x++;
//				obj->AnimStart();
			}
			if(keyMessage->btn & KEY_UP){
				obj->Pos.z++;
			}
			if(keyMessage->btn & KEY_DOWN){
				obj->Pos.z--;
			}
			if(keyMessage->cmd & NEW_A){
				obj->SetAnimIndex(3);
			}

}
void AnimPlayer::Relativize(D3DXVECTOR3* stick_rel,const D3DXVECTOR3* stick_abs,float* rot_rel,const float* rot_abs){

	//�J�����̉�]�����߂�
	D3DXVECTOR3 camVec = cam->lookatPt - cam->eyePt;
	float camRot = atan2f(camVec.x,camVec.z);

	//�J��������]�����đ��Ή�
	D3DXMATRIX around;
	D3DXMatrixRotationY(&around,camRot);
	D3DXVec3TransformCoord(stick_rel,stick_abs,&around);

	//��]�𑊑Ή�
	*rot_rel = camRot + *rot_abs;
}
void AnimPlayer::MoveRelatively(KeyMessage* keyMessage){

//	obj->ResetVelocity();

/*	//�ړ�
	//�Е��̒l�̐�Βl���������l�𒴂��Ă����ꍇ�̓X�e�B�b�N���͂��������ƔF������
	if(	(fabs((double)keyMessage->rightleft)>JS_DEAD
		|| fabs((double)keyMessage->updown)>JS_DEAD)
		&&obj->GetAnimChange()
		)
	{
		stickVec = D3DXVECTOR3(keyMessage->rightleft,0.0f,-keyMessage->updown);
		
		D3DXVec3Normalize(&stickVec,&stickVec);
		rot = atan2f(-stickVec.x,-stickVec.z);

			//�������l(�R�T�O)�`�T�O�O
		if(fabs((double)keyMessage->rightleft)>800
			||fabs((double)keyMessage->updown)>800)
		{
			stickVec *= MOVE_SPEED_FAST;
			obj->SetAnimIndex(1);
			obj->SetAnimSpeed(2.0f);
		}
		else if(fabs((double)keyMessage->rightleft)>600
			||fabs((double)keyMessage->updown)>600)
		{
			stickVec *= MOVE_SPEED_NORMAL;
			obj->SetAnimIndex(1);
			obj->SetAnimSpeed(1.0f);
		}
		else{
			stickVec *= MOVE_SPEED_SLOW;
			obj->SetAnimIndex(0);
			obj->SetAnimSpeed(1.0f);
		}*/
//	}else{
//		if(obj->GetAnimChange()){
//			obj->AnimStop();
//		}
//	}

	float rot=0;

	D3DXVECTOR3 stickVec=D3DXVECTOR3(0,0,0);
	if(!nowJump){
		GetStickVec(keyMessage,&stickVec,&rot);
		Relativize(&stickVec,&stickVec,&rot,&rot);

		//�X�e�B�b�N�������Ă���΃X�e�B�b�N�ɉ����Ċp�x��ς���
		if(D3DXVec3Length(&stickVec)>0){
			obj->Yaw= rot;
		}

		obj->Velo += stickVec;
	}


	//�W�����v
	if(keyMessage->btn & KEY_A){//A�������ꂽ
		if(keyMessage->cmd & NEW_A && !nowJump && obj->GetAnimChange()){//�W�����v�J�n
			jumpSpeed = JUMP_SPEED;//�X�s�[�h��ݒ�
			nowJump = true;//�W�����v���ɐݒ�
		}
		if(nowJump){//�W�����v���Ȃ�
			obj->Velo.y	+= jumpSpeed;
			jumpSpeed	-= 0.05f;//����
			if(jumpSpeed < 0){//�����J�n�Ȃ�
				jumpSpeed	= 0;
			}
		}
	}else{
		jumpSpeed = 0;
	}

	//MyOutputDebugString("Jump:%d Speed:%f\n",nowJump, jumpSpeed);

	difference = stickVec;
}
void AnimPlayer::FlyInTheSky(KeyMessage* keyMessage){}
void AnimPlayer::Gravity(){
}
void AnimPlayer::Move(KeyMessage* keyMessage){
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