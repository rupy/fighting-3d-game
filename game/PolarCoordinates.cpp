#include "PolarCoordinates.h"

PolarCoordinates::PolarCoordinates(vector<Effect*>& eff,
								   LPDIRECT3DTEXTURE9 tex,
								   bool mode,
								   float height,
								   float radius,
								   float theta,
								   float phi
								   )
:BillboardObject(eff,tex),
mode(mode),height(height),radius(radius),theta(theta),phi(phi)
{
}
PolarCoordinates::~PolarCoordinates(){
}
D3DXMATRIXA16 PolarCoordinates::MatrixSetting(){
	D3DXMATRIXA16 matPos;		//�ʒu
	D3DXMATRIXA16 matRot;		//��]
	D3DXMATRIXA16 matScale;		//�X�P�[��
	D3DXMATRIXA16 matResult;	//����
	D3DXMATRIXA16 matParent;	//�e�̍s��

	D3DXMatrixIdentity(&matPos);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);
	D3DXMatrixIdentity(&matParent);

	//�e�����Ă��L���Ȃ�e���v�Z����
	if(parentObj != NULL && validParent){
		matParent *= parentObj->MatrixSetting();
	}

	matPos = mode?Cylindrical():Spherical();
	D3DXMatrixRotationYawPitchRoll(&matRot, Yaw, Roll, Pitch);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matResult = matScale * matRot * matPos * matParent;

	return matResult;
}

D3DXMATRIXA16 PolarCoordinates::BillboardMatrixSetting(){
		D3DXMATRIXA16 matPos;		//�ʒu
	D3DXMATRIXA16 matRot;		//��]
	D3DXMATRIXA16 matScale;		//�X�P�[��
	D3DXMATRIXA16 matResult;	//����
	D3DXMATRIXA16 matParent;	//�e�̍s��

	D3DXMatrixIdentity(&matPos);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);
	D3DXMatrixIdentity(&matParent);

	//�e�����Ă��L���Ȃ�e���v�Z����
	if(parentObj != NULL && validParent){
		D3DXMATRIXA16 parentmat = parentObj->MatrixSetting();
		matParent._41 = parentmat._41;
		matParent._42 = parentmat._42;
		matParent._43 = parentmat._43;
	}

	matPos = mode?Cylindrical():Spherical();
	//CalcLookAtMatrix(&matRot, &Pos, &lookatPt, &upVec);
	D3DXMatrixLookAtLH(&matRot, &D3DXVECTOR3(0,0,0), &lookatPt, &upVec);
	D3DXMatrixInverse(&matRot, NULL, &matRot);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matResult = matScale * matRot * matPos * matParent;

	return matResult;

}
D3DXMATRIXA16 PolarCoordinates::Cylindrical(){

	D3DXMATRIXA16 matPos;		//�ʒu
	D3DXMatrixIdentity(&matPos);
	D3DXMatrixTranslation(&matPos,
		radius * cosf(theta),
		height,
		radius * sinf(theta));

	return matPos;

}
D3DXMATRIXA16 PolarCoordinates::Spherical(){

	D3DXMATRIXA16 matPos;		//�ʒu
	D3DXMatrixIdentity(&matPos);
	D3DXMatrixTranslation(&matPos,
		radius * cosf(theta) * cosf(phi),
		radius * sinf(phi),
		radius * sinf(theta) * cosf(phi));

	return matPos;

}