#ifndef __STATUS_MANU2_H__
#define __STATUS_MANU2_H__

#include "stringObject.h"
#include "numberObject.h"
#include "status.h"

class StatusManu:public ResourceBase{
public:
	
	LPDIRECT3DTEXTURE9 titlePic;
	LPDIRECT3DTEXTURE9 curPic;
	LPDIRECT3DTEXTURE9 iconAPic;
	SpriteObject* title;
	SpriteObject* cur;
	SpriteObject* iconA;

	StringObject_S* strHP;
	StringObject_S* strMP;
	StringObject_S* strEXP;
	StringObject_S* strAttack;
	StringObject_S* strDefense;
	StringObject_S* strSkill;

	NumberObject2* numHP;
	NumberObject2* numMP;
	NumberObject2* numEXP;
	NumberObject2* numAttack;
	NumberObject2* numDefense;
	NumberObject2* numSkill;
protected:
	enum CurPos{
		NAME,
		HP,
		MP,
		EXP,
		ATTACK,
		DEFENSE,
		SKILL,
	};

	CurPos curPos;

	LPD3DXSPRITE pSprite;

public:	
	StatusManu(LPD3DXSPRITE sprite);
	~StatusManu();

	void SetStatus(Status* st);

	HRESULT setupTextures();
	void Appear();

};


#endif	/*__STATUS_MANU2_H__*/