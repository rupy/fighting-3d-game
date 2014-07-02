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
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「ゲームを終了しますか？」")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("選択肢\n　・いいえ\n　・はい")));
	msg.back().select.push_back("noexit");
	msg.back().select.push_back("exit");
	msgSet.insert(make_pair("start",msg));

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「終了します」")));
	msg.back().nextaction = -1;
	msgSet.insert(make_pair("exit",msg));

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「再開します」")));
	msgSet.insert(make_pair("noexit",msg));
	msg.back().nextaction = 0;

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「これは車ですね。」")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「乗りますか？」")));
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("選択肢\n　・乗る\n　・乗らない")));
	msg.back().select.push_back("car_ride");
	msg.back().select.push_back("car_no");
	msgSet.insert(make_pair("car",msg));


	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「車に乗ります」")));
	msgSet.insert(make_pair("car_ride",msg));
	msg.back().nextaction = 0;

	msg.clear();
	msg.push_back(StringObject(D3DXVECTOR3(20,150,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,1,_T("【るか】\n「車に乗りません」")));
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
	if(itMsg->select.size()>0){//選択肢なら
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
		if(itMsg != itMsgSet->second.begin()){//前がある
			itMsg--;
			msgWait = !msgWait;
			return TRUE;
		}else{//前がない
			return FALSE;
		}
	}
	return TRUE;


}
bool MsgBox::NextMsg(int* nextaction){

	*nextaction = 0;
	if(!msgWait&&!disappear){
		if(itMsg != itMsgSet->second.end()-1){//次がある
			msgWait = !msgWait;
			itMsg++;
			return TRUE;
		}else{//次がない
			disappear = TRUE;
			noNext=TRUE;
			if(itMsg->select.size()>0){//選択肢なら
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

	if(!noNext){//終わっていなければ変更可能
		disappear = !disappear;
		box->color = D3DCOLOR_ARGB(200,255,255,255);
	}
}
void MsgBox::FadeOut(){
	
	size_t hidespeed=0x10;
	if(!box->color){//0なら終了
		return;
	}

	box->color -= hidespeed<<24;//アルファ値を減らす
	if(box->color < hidespeed<<24){//次に減らした時に0を過ぎてしまう場合
		box->color=0;//0にする
	}

}

//=============================================================
// テクスチャの読み込み
//=============================================================
HRESULT MsgBox::setupTextures(){

	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/msgBox.jpg", &boxTex))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/wait.png", &nextTex))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}


	return(S_OK);
}