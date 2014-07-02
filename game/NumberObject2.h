#ifndef __NUMBER_OBJECT_H__
#define __NUMBER_OBJECT_H__

#include "Object.h"
#include "Font.h"
#include <vector>
#include <string>
#include "MySpriteObject.h"
using namespace std;

class NumberObject:public MySpriteObject
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
	D3DXCOLOR		fontColor;

	D3DXVECTOR3		basePos;


protected:

public:

	NumberObject(
		int fontSize,
		int fontFamily,
		int letterSpacing,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		D3DXCOLOR color=D3DCOLOR_ARGB(255,255,255,255)
		);

	~NumberObject();
	//é©ï™Çï`âÊ
	void SetNumber(int number);
	void SetNumber(float number,int decimalPlace);
	void SetNumber(double number,int decimalPlace);
	void Appear();

};

#endif	/*__NUMBER_OBJECT_H__*/