#include "MsgBox.h"
#include "Common.h"

MsgBox::MsgBox()
:box(NULL),next(NULL),boxTex(NULL),nextTex(NULL),cur(32,32,CPF_LEFT_BOTTOM,0,D3DXVECTOR3(20,90,0)),select(0)
{

	msgWait=TRUE;
	disappear=FALSE;
	noNext=FALSE;

	setupTextures();

	vector<StringObject> msg;
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u�Q�[�����I�����܂����H�v")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("�I����\n�@�E������\n�@�E�͂�")));
	msg.back().select.push_back("noexit");
	msg.back().select.push_back("exit");
	msgSet.insert(make_pair("start",msg));

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u�I�����܂��v")));
	msg.back().nextaction = -1;
	msgSet.insert(make_pair("exit",msg));

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u�ĊJ���܂��v")));
	msgSet.insert(make_pair("noexit",msg));
	msg.back().nextaction = 0;

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u����͎Ԃł��ˁB�v")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u���܂����H�v")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("�I����\n�@�E���\n�@�E���Ȃ�")));
	msg.back().select.push_back("car_ride");
	msg.back().select.push_back("car_no");
	msgSet.insert(make_pair("car",msg));


	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u�Ԃɏ��܂��v")));
	msgSet.insert(make_pair("car_ride",msg));
	msg.back().nextaction = 0;

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("�y�邩�z\n�u�Ԃɏ��܂���v")));
	msgSet.insert(make_pair("car_no",msg));
	msg.back().nextaction = 0;

	itMsgSet = msgSet.begin();
	itMsg = itMsgSet->second.begin();

	box = new MySpriteObject(800,179,CPF_LEFT_BOTTOM,boxTex);
	next = new MySpriteObject(16,16,CPF_LEFT_TOP,nextTex);
	box->color = D3DCOLOR_ARGB(200,255,255,255);
	cur.SetupTexture("picture/select.png");
}

MsgBox::~MsgBox(){
	SAFE_DELETE(box);
	SAFE_DELETE(next);

	SAFE_RELEASE(boxTex);
	SAFE_RELEASE(nextTex);
}

bool MsgBox::SelectMsg(const char* msg_name){
	itMsgSet = msgSet.find(msg_name);
	itMsg = itMsgSet->second.begin();
	itMsg->cur = 0;
	itMsg->displayEnd = false;
	msgWait=TRUE;
	disappear=FALSE;
	noNext=FALSE;
	box->color = D3DCOLOR_ARGB(200,255,255,255);
	return true;
}
void MsgBox::Move(KeyMessage* keyMessage){
	if(itMsg->select.size()>0){//�I�����Ȃ�
		if(keyMessage->cmd & NEW_DOWN && (size_t)select+1 <itMsg->select.size()){
			select++;
			cur.Pos.y -= 25.0f;
		}else if(keyMessage->cmd & NEW_UP && 0 < select){
			select--;
			cur.Pos.y += 25.0f;
		}
	}
}
bool MsgBox::PrevMsg(){

	if(!msgWait&&!disappear){
		if(itMsg != itMsgSet->second.begin()){//�O������
			itMsg--;
			msgWait = !msgWait;
			return TRUE;
		}else{//�O���Ȃ�
			return FALSE;
		}
	}
	return TRUE;


}
bool MsgBox::NextMsg(int* nextaction){

	*nextaction = 0;
	if(!msgWait&&!disappear){
		if(itMsg != itMsgSet->second.end()-1){//��������
			msgWait = !msgWait;
			itMsg++;
			return TRUE;
		}else{//�����Ȃ�
			disappear = TRUE;
			noNext=TRUE;
			if(itMsg->select.size()>0){//�I�����Ȃ�
				SelectMsg(itMsg->select[select].c_str());
				return TRUE;
			}
			cur.Pos = D3DXVECTOR3(20,90,0);
			select = 0;
			*nextaction = itMsg->nextaction;
			return FALSE;
		}
	}
	return TRUE;

}
void MsgBox::Appear(){

	box->Appear();

	if(disappear){
		FadeOut();
		return;
	}


	itMsg->Appear();

	if(itMsg->displayEnd){
		msgWait = FALSE;

		next->Pos = itMsg->endPos;
		next->Pos.y -= 10.0f;
		next->Appear();
		itMsg->select.size()>0?cur.Appear():0;
	}
}
void MsgBox::ChangeHide(){

	if(!noNext){//�I����Ă��Ȃ���ΕύX�\
		disappear = !disappear;
		box->color = D3DCOLOR_ARGB(200,255,255,255);
	}
}
void MsgBox::FadeOut(){
	
	size_t hidespeed=0x10;
	if(!box->color){//0�Ȃ�I��
		return;
	}

	box->color -= hidespeed<<24;//�A���t�@�l�����炷
	if(box->color < hidespeed<<24){//���Ɍ��炵������0���߂��Ă��܂��ꍇ
		box->color=0;//0�ɂ���
	}

}

//=============================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================
HRESULT MsgBox::setupTextures(){

	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/msgBox.jpg", &boxTex))){
		MessageBox( NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/wait.png", &nextTex))){
		MessageBox( NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}


	return(S_OK);
}