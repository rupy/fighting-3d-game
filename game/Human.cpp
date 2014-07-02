#include "Human.h"

Human::Human(Camera* cam,AnimObject* obj)
:Player(cam),obj(obj),mode(MOVE_REL),moveVelo(0,0,0),nowJump(false),jumpSpeed(0),jumpAccel(-0.03f)
{
}

Human::~Human(){
}

void Human::GetStickVec(KeyMessage* keyMessage,D3DXVECTOR3* stick,float* cam_rot){
	D3DXVECTOR3 stickVec(0.0f,0.0f,0.0f);
	float rot = 0;

	//�Е��̒l�̐�Βl���������l�𒴂��Ă����ꍇ�̓X�e�B�b�N���͂��������ƔF������
	if(	(fabs((double)keyMessage->rightleft)>JS_DEAD
		|| fabs((double)keyMessage->updown)>JS_DEAD)
		&&obj->GetAnimChange()
		)
	{
		stickVec = D3DXVECTOR3(keyMessage->rightleft,0.0f,-keyMessage->updown);

		rot = atan2f(-stickVec.x,-stickVec.z);

		stickVec /= (float)JS_RESO;//�X�e�B�b�N�̃x�N�g����0����1�ɃN�����v
		stickVec *= HUMAN_SPEED;
	}


	*stick = stickVec;
	*cam_rot = rot;

}


void Human::Relativize(D3DXVECTOR3* stick_rel,const D3DXVECTOR3* stick_abs,float* rot_rel,const float* rot_abs){

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
void Human::Jump(KeyMessage* keyMessage){
	//�W�����v
	if(obj->top){
		jumpSpeed = 0;
	}
	if(keyMessage->btn & KEY_A){//A�������ꂽ
		if(keyMessage->cmd & NEW_A && !nowJump && obj->GetAnimChange()){//�W�����v�J�n
			jumpSpeed = JUMP_SPEED;//�X�s�[�h��ݒ�
			nowJump = true;//�W�����v���ɐݒ�
		}
		if(nowJump){//�W�����v���Ȃ�
			obj->Velo.y	+= jumpSpeed;
			jumpSpeed	+= jumpAccel;//����
			if( jumpSpeed < 0){//�����J�n�Ȃ�
				jumpSpeed	= 0;
			}
		}
	}else{
		jumpSpeed = 0;
	}
}
void Human::Animation(){

	if(!obj->GetAnimChange()){
		return;
	}
	float speed = D3DXVec3Length(&(moveVelo/HUMAN_SPEED));
	switch(mode){
		case MOVE_ABS:
		case MOVE_REL:
			//�A�j���[�V����
			if(nowJump){
				obj->AnimStop();
			}else if(speed >0.8f){
				obj->SetAnimIndex(1);
				obj->SetAnimSpeed(2.0f);
			}else if(speed > 0.5f){
				obj->SetAnimIndex(1);
				obj->SetAnimSpeed(1.0f);
			}else if(speed > 0.2f){
				obj->SetAnimIndex(0);
				obj->SetAnimSpeed(1.0f);
			}else{
				obj->AnimStop();
			}
			break;
		case MOVE_SWIM:
			if(speed >0.8f){
				obj->SetAnimIndex(10);
				obj->SetAnimSpeed(1.0f);
			}else if(speed > 0.5f){
				obj->SetAnimIndex(10);
				obj->SetAnimSpeed(0.5f);
			}else{
				obj->AnimStop();
			}
			//MyOutputDebugString("[%f,%f,%f]%f\n",moveVelo.x,moveVelo.y,moveVelo.z,speed);
			break;
	}
}
void Human::MoveRelatively(KeyMessage* keyMessage){

	obj->ResetVelocity();

	float rot=0;

	D3DXVECTOR3 stickVec=D3DXVECTOR3(0,0,0);
	if(!nowJump){
		GetStickVec(keyMessage,&stickVec,&rot);
		Relativize(&stickVec,&stickVec,&rot,&rot);

		//�X�e�B�b�N�������Ă���΃X�e�B�b�N�ɉ����Ċp�x��ς���
		if(D3DXVec3Length(&stickVec)>0){
			obj->Yaw= rot;
		}
		moveVelo = stickVec;
	}

	Jump(keyMessage);

	obj->Velo += moveVelo;

	Animation();

	if(keyMessage->cmd & NEW_B && obj->GetAnimChange()){
		obj->SetAnimIndex(8);
		obj->SetAnimSpeed(1);
		obj->NoAnimChange();
	}
	//MyOutputDebugString("Jump:%d Speed:%f\n",nowJump, jumpSpeed);

	Gravity();
	if(obj->Pos.y<0){
		obj->Velo.y = 0;
		mode = MOVE_SWIM;
		nowJump = false;
		obj->SetAnimChange(true);
	}
}
void Human::MoveAbsolutely(KeyMessage* keyMessage){
	obj->ResetVelocity();

	float rot=0;
	moveVelo *= DECAY_RATE;

	D3DXVECTOR3 stickVec=D3DXVECTOR3(0,0,0);
	if(!nowJump){
		GetStickVec(keyMessage,&stickVec,&rot);
		//		Relativize(&stickVec,&stickVec,&rot,&rot);

		//�X�e�B�b�N�������Ă���΃X�e�B�b�N�ɉ����Ċp�x��ς���
		if(D3DXVec3Length(&stickVec)>0){
			obj->Yaw= rot;
		}
		stickVec;
	}

	Jump(keyMessage);

	obj->Velo += moveVelo;

	Animation();

	//MyOutputDebugString("Jump:%d Speed:%f\n",nowJump, jumpSpeed);

	Gravity();
	
	if(obj->Pos.y<0){
		obj->Velo.y = 0;
		mode = MOVE_SWIM;
		nowJump = false;
		obj->SetAnimChange(true);
	}
}
void Human::Swim(KeyMessage* keyMessage){

	obj->ResetVelocity();

	float rot=0;
	moveVelo *= DECAY_RATE;

	D3DXVECTOR3 stickVec=D3DXVECTOR3(0,0,0);
	GetStickVec(keyMessage,&stickVec,&rot);
	Relativize(&stickVec,&stickVec,&rot,&rot);

	moveVelo = stickVec;
	moveVelo.y = SWIM_SPEED;


	//�X�e�B�b�N�������Ă���΃X�e�B�b�N�ɉ����Ċp�x��ς���
	if(D3DXVec3Length(&stickVec)>0){
		obj->Yaw= rot;
	}

	Animation();
	if(keyMessage->btn & KEY_B && obj->GetAnimChange() ){
		moveVelo.y = -SWIM_SPEED;
	}
	if(obj->GetAnimChange() ){
		obj->SetAnimIndex(10);
	}
	obj->Velo += moveVelo;

	//obj->Roll = atan2f(0,moveVelo.y);

	Surface();/*
	if(obj->Pos.y>-0.5f&&keyMessage->btn & KEY_A){
	}*/
}

void Human::FlyInTheSky(KeyMessage* keyMessage){}
void Human::Gravity(){
	obj->Gravity(0.3f);
}
void Human::Surface(){
	//�C��荂���j�������ɂȂ�����h��
	if(obj->Velo.y + obj->Pos.y>0){
		obj->Velo.y = 0;
	}
	//�n�ʂɒ��������������B
	if(obj->land && obj->Pos.y>0){
		mode = MOVE_REL;
		obj->SetAnimChange(true);
	}
}
void Human::Move(KeyMessage* keyMessage){

	switch(mode){
		case MOVE_ABS:
			MoveAbsolutely(keyMessage);
			break;
		case MOVE_REL:
			MoveRelatively(keyMessage);
			break;
		case MOVE_FLY:
			break;
		case MOVE_SWIM:
			Swim(keyMessage);
			break;
		default:
			break;
	}
	//MyOutputDebugString("mode:%d\n",mode);

}