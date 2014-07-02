#ifndef __NUMBER_OBJECT2_H__
#define __NUMBER_OBJECT2_H__

#include "Object.h"
#include "Font.h"
#include <vector>
#include <string>
using namespace std;

class NumberObject2:public Object
{
	union{
		int		iNum;
		double	dNum;
	} uNumber;

	int				decimalPlace;//è¨êîì_à»â∫ÇÃåÖêî

	vector <Font>	font;
	int				fontFamily;
	int				fontSize;
	int				letterSpacing;

	D3DXVECTOR3		curPos;
	RECT			uvRect;

protected:
	LPD3DXSPRITE	pSprite;

public:

	NumberObject2(
		LPD3DXSPRITE sprite,
		int fontSize,
		int fontFamily,
		int letterSpacing,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DCOLOR color=D3DCOLOR_ARGB(255,255,255,255)
		);

	~NumberObject2();
	//é©ï™Çï`âÊ
	void SetNumber(int number);
	void SetNumber(float number,int decimalPlace);
	void SetNumber(double number,int decimalPlace);
	void Appear();

};

#endif	/*__NUMBER_OBJECT2_H__*/