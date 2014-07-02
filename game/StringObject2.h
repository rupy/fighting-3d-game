#ifndef __STRINGOBJECT_H__
#define __STRINGOBJECT_H__

#include "MySpriteObject.h"
#include "Font.h"
#include "SpriteObject.h"
#include <vector>

using namespace std;


class StringObject :public MySpriteObject
{
public:
	vector <Font> font;
	int fontFamily;
	int fontSize;
	bool changeLine;//改行するかどうか
	float indent;
	float lineHeight;
	float letterSpacing;
	float rightEdge;
	D3DXCOLOR fontColor;

	int speed;
	int time;
	int cur;
	float maxLine;

	D3DXVECTOR3 basePos;
	D3DXVECTOR3 endPos;

	bool displayEnd;

	//付加情報（新しくクラス作るべきだが、時間もないのでここに）
	vector<string> select;//添え字が選択肢番号、値の文字列は次のメッセージセットの名前。
	int nextaction;//メッセージを終えたときの次の行動の番号

protected:

public:

	StringObject(D3DXVECTOR3 pos,
		float rightEdge,
		int fontsize,
		int fontFamily,
		D3DXCOLOR color,
		bool changeline,
		float indent,
		float lineHeight,
		float letterSpacing,
		int speed,
		const TCHAR t[]);

	~StringObject();
	int SetSize(const TCHAR* c);
	//自分を描画
	void Appear();

};
#endif	/*__STRINGOBJECT_H__*/