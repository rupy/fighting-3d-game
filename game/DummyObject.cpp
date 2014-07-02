#include "DummyObject.h"

DummyObject::DummyObject(D3DXVECTOR3 pos,
						 float Y,float R,float P)
:Object(pos,Y,R,P)
{
}
D3DXMATRIXA16 DummyObject::MatrixSetting(){
	D3DXMATRIXA16 matResult = Object::MatrixSetting();
		return matResult;
}
DummyObject::~DummyObject(){
}
void DummyObject::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_XYZ);

	D3DMATERIAL9 material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse.a = 255;
	material.Diffuse.r = 255;
	material.Diffuse.g = 255;
	material.Diffuse.b = 255;
	material.Ambient = material.Diffuse;
	material.Emissive = material.Diffuse;

	GetDevice()->SetMaterial(&material);

	D3DXVECTOR3 vecPoint[3];

	vecPoint[0] = D3DXVECTOR3(-1.0f,0,0);
	vecPoint[1] = D3DXVECTOR3(1.0f,0,0);
	GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
	
	vecPoint[0] = D3DXVECTOR3(0,0,-1.0f);
	vecPoint[1] = D3DXVECTOR3(0,0,1.0f);
	GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));

}