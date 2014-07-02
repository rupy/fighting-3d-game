#ifndef __MSGBOX_H__
#define __MSGBOX_H__

#include "StringObject2.h"
#include <vector>
#include <map>
#include <string>
#include "Input.h"

using namespace std;

class MsgBox:public ResourceBase{
public:	

	
	LPDIRECT3DTEXTURE9 boxTex;
	LPDIRECT3DTEXTURE9 nextTex;
	MySpriteObject* box;
	MySpriteObject* next;
	MySpriteObject cur;

	map<string, vector<StringObject>> msgSet;
	bool msgWait;
	bool disappear;
	bool noNext;
	int select;

protected:
	
	map<string, vector<StringObject>>::iterator	itMsgSet;
	vector<StringObject>::iterator	itMsg;


public:
	MsgBox();
	~MsgBox();

	bool SetMsg(const char* msg_name);//���b�Z�[�W���Z�b�g
	bool SelectMsg(const char* msg_name);//���b�Z�[�W��I��
	bool PrevMsg();//���̃��b�Z�[�W�֍s��
	bool NextMsg(int* nextaction);//�O�̃��b�Z�[�W�֍s��
	void FadeOut();//�{�b�N�X������
	void ChangeHide();
	void Move(KeyMessage* keyMessage);

	void Appear();
	HRESULT setupTextures();

};


#endif	/*__MSGBOX_H__*/