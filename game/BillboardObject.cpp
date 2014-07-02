#include "BillboardObject.h"

BillboardObject::BillboardObject(vector<Effect*>& eff,
								 LPDIRECT3DTEXTURE9 tex,
								 D3DXVECTOR3 position,
								 float Y,float R,float P)
								 :VertexObject(eff,tex,position,Y,R,P)
{
}
BillboardObject::~BillboardObject(){}


void BillboardObject::LookAtMe(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){

	D3DXVECTOR3 dir = *look_pos - *cam_pos;
	this->lookatPt = dir;
	this->upVec    =D3DXVECTOR3(0,1.0f,0);
}
void BillboardObject::Appear(){

	D3DXMATRIXA16 matWorld = BillboardMatrixSetting();

	GetDevice()->SetVertexShader(NULL);

	GetDevice()->SetStreamSource( 0, pMyVB, 0, sizeof(MY_VERTEX) );

	GetDevice()->SetFVF(MY_VERTEX_FVF);

	GetDevice()->SetTexture(0,pTex);
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);


	// •`‰æ
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );


}