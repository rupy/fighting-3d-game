#include "AnimCPU.h"

AnimCPU::AnimCPU(AnimObject* obj)
:obj(obj),mode(aaaa),speed(0.1f),nowJump(false),territory(10.0f),difference(D3DXVECTOR3(0,0,0)),HP(15),jumptime(200)
{
}
//ここらへんは時間がないので・・・ぐじゃぐじゃなのは勘弁してください（汗
void AnimCPU::Move(){
	obj->ResetVelocity();
	if(HP == 0){
		jumptime--;
		if(jumptime < -100){
			mode=bbb;
			HP = 20;
			speed+=0.05f;
		}
	}
	switch(mode){
		case 0:
		case 1:
			if(CheckTerritory()&&obj->GetAnimChange()){
				obj->SetAnimIndex(1);
				obj->AnimStart();
				RunAfterTarget();
				obj->OkAnimChange();

				if(mode == 1){
					if(RAND_0_1() < 9)
						jumptime = 10;

					//ジャンプ
					if(jumptime>9 ){//Aが押された
						if( jumptime >9&&!nowJump && obj->GetAnimChange()){//ジャンプ開始
							jumpSpeed = 0.6f;//スピードを設定
							nowJump = true;//ジャンプ中に設定
							jumptime = 9;
						}
						if(nowJump){//ジャンプ中なら
							jumptime--;
							obj->Velo.y	+= jumpSpeed;
							jumpSpeed	-= 0.05f;//減速
							if(jumpSpeed < 0){//落下開始なら
								jumpSpeed	= 0;
								nowJump = false;
							}
						}
					}
				}else{
					jumpSpeed = 0;
				}
			}else{
				if(obj->GetAnimChange()){
					float a = RAND_0_1();
					if(a<0.2f){
						obj->SetAnimIndex(3);
					}
					else if(a<0.4f){
						obj->SetAnimIndex(5);
					}
					else if(a<0.6f){
						obj->SetAnimIndex(6);
					}
					obj->NoAnimChange();
					obj->SetAnimSpeed(2);
					obj->AnimStart();
				}
			}
			break;
		default:
			break;
	}
	Gravity();
}
void AnimCPU::RunAfterTarget(){
	D3DXVECTOR3 vecDir = targetObj->Pos - obj->Pos;
	obj->Yaw=atan2f(-vecDir.x,-vecDir.z);
	vecDir.y = 0;
	D3DXVec3Normalize(&vecDir,&vecDir);
	obj->Velo += vecDir*speed;
	difference = vecDir*speed;
}
void AnimCPU::ComeAfterTarget(){
	D3DXVECTOR3 vecDir = targetObj->Pos - obj->Pos;
	D3DXVec3Normalize(&vecDir,&vecDir);
	obj->Velo += vecDir*speed;
	difference = vecDir*speed;
}
bool AnimCPU::CheckTerritory(){
	D3DXVECTOR3 vecDir=targetObj->Pos - obj->Pos;
	vecDir.y=0;
	float dir = D3DXVec3Length(&vecDir);
	if(dir > 1.0f){
		return true;
	}
	return false;
}
void AnimCPU::SetTargetObject(Object* obj){
	targetObj=obj;
}

void AnimCPU::Gravity(){
	obj->Gravity(0.3f);
}