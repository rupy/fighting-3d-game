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

	//片方の値の絶対値がしきい値を超えていた場合はスティック入力があったと認識する
	if(	(fabs((double)keyMessage->rightleft)>JS_DEAD
		|| fabs((double)keyMessage->updown)>JS_DEAD)
		&&obj->GetAnimChange()
		)
	{
		stickVec = D3DXVECTOR3(keyMessage->rightleft,0.0f,-keyMessage->updown);
		
		D3DXVec3Normalize(&stickVec,&stickVec);
		rot = atan2f(-stickVec.x,-stickVec.z);

			//しきい値(３５０)～５００
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

	//カメラの回転を求める
	D3DXVECTOR3 camVec = cam->lookatPt - cam->eyePt;
	float camRot = atan2f(camVec.x,camVec.z);

	//カメラ分回転させて相対化
	D3DXMATRIX around;
	D3DXMatrixRotationY(&around,camRot);
	D3DXVec3TransformCoord(stick_rel,stick_abs,&around);

	//回転を相対化
	*rot_rel = camRot + *rot_abs;
}
void AnimPlayer::MoveRelatively(KeyMessage* keyMessage){

//	obj->ResetVelocity();

/*	//移動
	//片方の値の絶対値がしきい値を超えていた場合はスティック入力があったと認識する
	if(	(fabs((double)keyMessage->rightleft)>JS_DEAD
		|| fabs((double)keyMessage->updown)>JS_DEAD)
		&&obj->GetAnimChange()
		)
	{
		stickVec = D3DXVECTOR3(keyMessage->rightleft,0.0f,-keyMessage->updown);
		
		D3DXVec3Normalize(&stickVec,&stickVec);
		rot = atan2f(-stickVec.x,-stickVec.z);

			//しきい値(３５０)～５００
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

		//スティックが入っていればスティックに応じて角度を変える
		if(D3DXVec3Length(&stickVec)>0){
			obj->Yaw= rot;
		}

		obj->Velo += stickVec;
	}


	//ジャンプ
	if(keyMessage->btn & KEY_A){//Aが押された
		if(keyMessage->cmd & NEW_A && !nowJump && obj->GetAnimChange()){//ジャンプ開始
			jumpSpeed = JUMP_SPEED;//スピードを設定
			nowJump = true;//ジャンプ中に設定
		}
		if(nowJump){//ジャンプ中なら
			obj->Velo.y	+= jumpSpeed;
			jumpSpeed	-= 0.05f;//減速
			if(jumpSpeed < 0){//落下開始なら
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