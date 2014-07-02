#include "Object.h"

Object::Object(D3DXVECTOR3 pos,
			   float Y,float R,float P,
			   D3DXVECTOR3 scale)
			   :Pos(pos),
			   Yaw(Y),Roll(R),Pitch(P),
			   Scale(scale),
			   parentObj(NULL),
			   lookatPt(D3DXVECTOR3(0,0,0)),
			   upVec(D3DXVECTOR3(0,0,0)),
			   Velo(D3DXVECTOR3(0,0,0)),
			   Accel(D3DXVECTOR3(0,0,0)),
			   top(false),land(false),wall(false),use_shadow(true),Inertia(0,0,0)
{
	//親がいたら有効にする
	if(!parentObj){
		validParent = true;
	}else{
		validParent = false;
	}
}

//Object::Object(LPDIRECT3DDEVICE9 device,
//	   D3DXVECTOR3 pos,
//	   D3DXVECTOR3 lookat,
//	   D3DXVECTOR3 upvec)
//	   :pDevice(device),
//	   Pos(pos),
//	   Yaw(0),Roll(0),Pitch(0),
//	   lookatPt(lookat),upVec(upVec),
//	   parentObj(NULL)
//{
//}

D3DXMATRIXA16 Object::MatrixSetting(){

	D3DXMATRIXA16 matPos;		//位置
	D3DXMATRIXA16 matRot;		//回転
	D3DXMATRIXA16 matScale;		//スケール
	D3DXMATRIXA16 matResult;	//結果
	D3DXMATRIXA16 matParent;	//親の行列

	D3DXMatrixIdentity(&matPos);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);
	D3DXMatrixIdentity(&matParent);

	//親がいてかつ有効なら親を計算する
	if(parentObj != NULL && validParent){
		matParent *= parentObj->MatrixSetting();
	}

	D3DXMatrixTranslation(&matPos, Pos.x, Pos.y, Pos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, Yaw, Roll, Pitch);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matResult = matScale * matRot * matPos * matParent;

	return matResult;

}
D3DXMATRIXA16 Object::BillboardMatrixSetting(){

	D3DXMATRIXA16 matPos;		//位置
	D3DXMATRIXA16 matRot;		//回転
	D3DXMATRIXA16 matScale;		//スケール
	D3DXMATRIXA16 matResult;	//結果
	D3DXMATRIXA16 matParent;	//親の行列

	D3DXMatrixIdentity(&matPos);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);
	D3DXMatrixIdentity(&matParent);

	//親がいてかつ有効なら親を計算する
	if(parentObj != NULL && validParent){
		D3DXMATRIXA16 parentmat = parentObj->MatrixSetting();
		matParent._41 = parentmat._41;
		matParent._42 = parentmat._42;
		matParent._43 = parentmat._43;
	}

	D3DXMatrixTranslation(&matPos, Pos.x, Pos.y, Pos.z);
	//CalcLookAtMatrix(&matRot, &Pos, &lookatPt, &upVec);
	D3DXMatrixLookAtLH(&matRot, &D3DXVECTOR3(0,0,0), &lookatPt, &upVec);
	D3DXMatrixInverse(&matRot, NULL, &matRot);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matResult = matScale * matRot * matPos * matParent;

	return matResult;

}
void Object::Shadow(){}

void Object::Move(){
	Pos += Velo;
	Inertia = Velo;
}
void Object::ResetVelocity(){
	Velo.x = Velo.y = Velo.z = 0;
}

void Object::SetParent(Object* parent){

	if(!parentObj){
		validParent = true;
		parentObj = parent;
	}else{
		validParent = false;
	}
}
void Object::Gravity(float gravity_weight){
	Velo.y -= gravity_weight;
}