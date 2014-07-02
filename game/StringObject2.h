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
	bool changeLine;//���s���邩�ǂ���
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

	//�t�����i�V�����N���X���ׂ������A���Ԃ��Ȃ��̂ł����Ɂj
	vector<string> select;//�Y�������I�����ԍ��A�l�̕�����͎��̃��b�Z�[�W�Z�b�g�̖��O�B
	int nextaction;//���b�Z�[�W���I�����Ƃ��̎��̍s���̔ԍ�

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
	//������`��
	void Appear();

};
#endif	/*__STRINGOBJECT_H__*/