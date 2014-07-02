#ifndef __SPRITEOBJECT_H__
#define __SPRITEOBJECT_H__

#include "Object.h"
#include "Font.h"


enum{
	LEFT_TOP,
	LEFT_CENTER,
	LEFT_BOTTOM,
	CENTER_TOP,
	CENTER,
	CENTER_BOTTOM,
	RIGHT_TOP,
	RIGHT_CENTER,
	RIGHT_BOTTOM,
	NOT_DEF
};

class SpriteObject :public Object
{
public:
	LPDIRECT3DTEXTURE9 pictureType;
	RECT uvRect;
	D3DXVECTOR3 vecCenter;
	D3DCOLOR color;
	int flag;

protected:
	LPD3DXSPRITE pSprite;

public:

	SpriteObject(LPD3DXSPRITE sprite,
		LPDIRECT3DTEXTURE9 pictureType,
		RECT uv,
		D3DXVECTOR3 center,
		int centerFlag,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		float Y=0.0f,float R=0.0f,float P=0.0f,
		D3DCOLOR color=D3DCOLOR_ARGB(255,255,255,255)
		);

	~SpriteObject();
	//Ž©•ª‚ð•`‰æ
	void Appear();

};
#endif	/*__SPRITEOBJECT_H__*/