#ifndef __STRINGOBJECT2_H__
#define __STRINGOBJECT2_H__

#include "Object.h"
#include "Font.h"
#include "SpriteObject.h"
#include <vector>

using namespace std;

int MultiByteStringLength(TCHAR* st);

class StringObject_S :public Object
{
public:
	vector <Font> font;
	RECT uvRect;
//	D3DXVECTOR3 vecCenter;
	D3DCOLOR color;
	int fontFamily;
	int fontSize;
	bool changeLine;//â¸çsÇ∑ÇÈÇ©Ç«Ç§Ç©
	float indent;
	float lineHeight;
	float letterSpacing;
	float rightEdge;

	int speed;
	int time;
	int cur;
	int maxLine;
	D3DXVECTOR3 curPos;

	bool displayEnd;

protected:
	LPD3DXSPRITE pSprite;

public:

	StringObject_S(LPD3DXSPRITE sprite,
		D3DXVECTOR3 pos,
		float rightEdge,
		int fontsize,
		int fontFamily,
		D3DXCOLOR color,
		bool changeline,
		float indent,
		float lineHeight,
		float letterSpacing,
		int speed,
		TCHAR t[]);

	~StringObject_S();
	int SetSize(TCHAR* c);
	//é©ï™Çï`âÊ
	void Appear();

};
#endif	/*__STRINGOBJECT2_H__*/