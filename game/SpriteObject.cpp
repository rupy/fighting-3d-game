#include "SpriteObject.h"


SpriteObject::SpriteObject(LPD3DXSPRITE sprite,
						   LPDIRECT3DTEXTURE9 picType,
						   RECT uv,
						   D3DXVECTOR3 center,
						   int centerFlag,
						   D3DXVECTOR3 position,
					   float Y,float R,float P,
					   D3DCOLOR color)
					   :Object(position,Y,R,P),pSprite(sprite),pictureType(picType),uvRect(uv),vecCenter(center),flag(centerFlag)
{
	this->color = color;


	switch(flag){

		case LEFT_TOP:
			vecCenter.x=(float)uv.left;
			vecCenter.y=(float)uv.top;
			break;
		case LEFT_CENTER:
			vecCenter.x=(float)uv.left;
			vecCenter.y=(float)(uv.bottom-uv.top)/2;
			break;
		case LEFT_BOTTOM:
			vecCenter.x=(float)uv.left;
			vecCenter.y=(float)uv.bottom;
			break;

		case CENTER_TOP:
			vecCenter.x=(float)(uv.right-uv.left)/2;
			vecCenter.y=(float)uv.top;
			break;
		case CENTER:
			vecCenter.x=(float)(uv.right-uv.left)/2;
			vecCenter.y=(float)(uv.bottom-uv.top)/2;
			break;
		case CENTER_BOTTOM:
			vecCenter.x=(float)(uv.right-uv.left)/2;
			vecCenter.y=(float)uv.bottom;
			break;

		case RIGHT_TOP:
			vecCenter.x=(float)uv.right;
			vecCenter.y=(float)uv.top;
			break;
		case RIGHT_CENTER:
			vecCenter.x=(float)uv.right;
			vecCenter.y=(float)(uv.bottom-uv.top)/2;
			break;
		case RIGHT_BOTTOM:
			vecCenter.x=(float)uv.right;
			vecCenter.y=(float)uv.bottom;
			break;
		default:
			break;
	}
			vecCenter.z=0.0f;
}
SpriteObject::~SpriteObject(){
}




void SpriteObject::Appear(){

	D3DXMATRIX mat;
	D3DXMATRIX mat2;
	D3DXMATRIX mat3;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixIdentity(&mat2);
	D3DXMatrixIdentity(&mat3);
	D3DXMatrixRotationZ(&mat, Roll);
	D3DXMatrixTranslation(&mat2,Pos.x,Pos.y,Pos.z);
	D3DXMatrixScaling(&mat3,0.5f,0.5f,0.5f);
	mat*=mat2;
//	mat*=mat3;

	pSprite->SetTransform(&mat);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_DONOTSAVESTATE);



	if(FAILED(pSprite->Draw(pictureType,
		&uvRect,
		&vecCenter,
		NULL,
		color))){
			//return(E_FAIL);
	}

	pSprite->End();
}