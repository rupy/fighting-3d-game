#include "Camera.h"
#include "Common.h"

Camera::Camera(vector<Effect*>& eff, D3DXVECTOR3 eye, D3DXVECTOR3 look, D3DXVECTOR3 up)
:rEff(eff),eyePt(eye),lookatPt(look),upVec(up),speed(8.0f),distance(15.0f),rSpeed(0.5f)
{
}
Camera::~Camera(){}


void Camera::SeeObject(const Object* obj){

//		eyePt.y = obj->Pos.y+3.0f;
		lookatPt = obj->Pos;

}
void Camera::RunAfterObject(const Object* obj){

	D3DXVECTOR3 objToCam = eyePt - obj->Pos;//�I�u�W�F�N�g����J�����ւ̃x�N�g��

	float l = D3DXVec3Length(&objToCam);//���ۂ̋���

	D3DXVECTOR3 target = objToCam * distance/l + obj->Pos;//�ړI�n���߂�

	//�J�������ړ�
	eyePt.x = target.x;
	eyePt.y = target.y;
	eyePt.z = target.z;

	SeeObject(obj);
}
void Camera::TurnRoundBehindObject(const Object* obj){

	D3DXVECTOR3 target = D3DXVECTOR3(
		obj->Pos.x - distance * cos(-PI_HALF - obj->Yaw),
		eyePt.y,
		obj->Pos.z - distance * sin(-PI_HALF - obj->Yaw)
		);
	float cam_to_target = D3DXVec3Length(&(target-eyePt));
//	MyOutputDebugString("<a:d:%f,%f>\n",cam_to_target,distance);

	if(fabs(target.x - eyePt.x)>1.0f||fabs(target.z - eyePt.z)>1.0f){
	//�ړI�n�֋������k�߂Ă���
		eyePt.x += (target.x - eyePt.x)/speed;
		eyePt.z += (target.z - eyePt.z)/speed;
//		eyePt.x += speed * ((target.x - eyePt.x)/cam_to_target);
//		eyePt.z += speed * ((target.z - eyePt.z)/cam_to_target);
		if(cam_to_target > distance+5.0){
			eyePt.x += (target.x - eyePt.x)/speed;
		}
	}


	//eyePt.x = target.x;
	//eyePt.z = target.z;

	SeeObject(obj);
}

void Camera::TurnRoundObjectR(const Object* obj ){

	D3DXVECTOR3 objToCam = eyePt - obj->Pos;//�I�u�W�F�N�g����J�����ւ̃x�N�g��
	objToCam.y = 0;
	D3DXVECTOR3 dist;
	
	D3DXMATRIX round;
	D3DXMatrixRotationY(&round, D3DXToRadian(-90.0f));//�����ȃx�N�g���𓾂�

	D3DXVec3TransformCoord(&dist,&objToCam,&round);

	D3DXVec3Normalize(&dist,&dist);

	eyePt += dist * rSpeed;//�E����

//	MyOutputDebugString( _T("<Zstick:%f,%f>\n"),dist.x,dist.z);
}
void Camera::TurnRoundObjectL(const Object* obj ){

	D3DXVECTOR3 objToCam = eyePt - obj->Pos;//�I�u�W�F�N�g����J�����ւ̃x�N�g��
	D3DXVECTOR3 dist;
	
	D3DXMATRIX round;
	D3DXMatrixRotationY(&round, D3DXToRadian(-90.0f));//�����ȃx�N�g���𓾂�

	D3DXVec3TransformCoord(&dist,&objToCam,&round);

	D3DXVec3Normalize(&dist,&dist);

	eyePt += dist * -rSpeed;//�����

//	MyOutputDebugString( _T("<Zstick:%f,%f>\n"),dist.x,dist.z);
}
void Camera::StayOverObject(const Object* obj,float height){
	eyePt.y = obj->Pos.y + height;
}

void Camera::CameraSetting(){

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &eyePt, &lookatPt, &upVec);

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9    vp;
	if(FAILED(GetDevice()->GetViewport(&vp))) {
//		return E_FAIL;
	}

	D3DXMATRIXA16 matProj;

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	D3DXMatrixPerspectiveFovLH(&matProj,D3DXToRadian(45.0f),aspect,1.0f,1000.0f);

	GetDevice()->SetTransform(D3DTS_VIEW, &matView);
	GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);

	if(FAILED(rEff[0]->SetMatrix(&matView,"g_view"))){
		MessageBox( NULL, "�ϐ��w��Ɏ��s", "�G���[", MB_OK|MB_ICONHAND );			
		PostQuitMessage(0);
	}
	if(FAILED(rEff[0]->SetMatrix(&matProj,"g_proj"))){
		MessageBox( NULL, "�ϐ��w��Ɏ��s", "�G���[", MB_OK|MB_ICONHAND );
		PostQuitMessage(0);
	}
}

void Camera::SetDistance(float distance){
	if(distance > ZOOM_MIN && distance < ZOOM_MAX)
		this->distance = distance;
}
void Camera::ZoomIn(float minus_dist){
	if(distance - minus_dist > ZOOM_MIN)
		distance -= minus_dist;
}
void Camera::ZoomOut(float plus_dist){
	if(distance + plus_dist < ZOOM_MAX)
		distance += plus_dist;
}

void Camera::SetSpeed(float speed){
	this->speed = speed;
}
void Camera::SetRSpeed(float rSpeed){
	this->rSpeed = rSpeed;
}
