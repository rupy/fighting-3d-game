#include "StatusMenu.h"
#include "Common.h"

StatusManu::StatusManu(LPD3DXSPRITE sprite)
:pSprite(sprite),
strHP(NULL),strMP(NULL),strEXP(NULL),strAttack(NULL),strDefense(NULL),strSkill(NULL),
numHP(NULL),numMP(NULL),numEXP(NULL),numAttack(NULL),numDefense(NULL),numSkill(NULL)
{
	if(FAILED(setupTextures())){
		return;
	}
	RECT rct =	 {0,0,800,600};
	title = new SpriteObject(sprite,titlePic,rct,D3DXVECTOR3(0,0,0),LEFT_TOP,D3DXVECTOR3(0,0,0));
	RECT rctn =	 {0,0,32,32};
	cur = new SpriteObject(sprite,curPic,rctn,D3DXVECTOR3(0,0,0),LEFT_TOP,D3DXVECTOR3(0,0,0));
	RECT rctn2 =	 {0,0,128,128};
	iconA = new SpriteObject(sprite,iconAPic,rctn2,D3DXVECTOR3(0,0,0),LEFT_TOP,D3DXVECTOR3(10,10,0));

	strHP		= new StringObject_S(sprite,D3DXVECTOR3(200,80,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("HP"));
	strMP		= new StringObject_S(sprite,D3DXVECTOR3(200,120,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("MP"));;
	strEXP		= new StringObject_S(sprite,D3DXVECTOR3(200,500,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("EXP"));;
	strAttack	= new StringObject_S(sprite,D3DXVECTOR3(200,180,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("ATTACK"));;
	strDefense	= new StringObject_S(sprite,D3DXVECTOR3(200,220,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("DIFENSE"));;
	strSkill	= new StringObject_S(sprite,D3DXVECTOR3(200,260,0),780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,0,_T("SKILL"));;

	numHP		= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,80,0),D3DXCOLOR(255,255,255,255));
	numMP		= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,120,0),D3DXCOLOR(255,255,255,255));
	numEXP		= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,500,0),D3DXCOLOR(255,255,255,255));
	numAttack	= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,180,0),D3DXCOLOR(255,255,255,255));
	numDefense	= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,220,0),D3DXCOLOR(255,255,255,255));
	numSkill	= new NumberObject2(sprite,25,0,0,D3DXVECTOR3(340,260,0),D3DXCOLOR(255,255,255,255));
	
}
StatusManu::~StatusManu(){
	SAFE_DELETE(title);
	SAFE_DELETE(cur);
	SAFE_DELETE(iconA);

	SAFE_DELETE(strHP);
	SAFE_DELETE(strMP);
	SAFE_DELETE(strEXP);
	SAFE_DELETE(strAttack);
	SAFE_DELETE(strDefense);
	SAFE_DELETE(strSkill);

	SAFE_DELETE(numHP);
	SAFE_DELETE(numMP);
	SAFE_DELETE(numEXP);
	SAFE_DELETE(numAttack);
	SAFE_DELETE(numDefense);
	SAFE_DELETE(numSkill);
}

void StatusManu::SetStatus(Status *st){
	
	numHP->SetNumber(st->GetHP());
	numMP->SetNumber(st->GetMP());
	numEXP->SetNumber(st->GetExp());
	numAttack->SetNumber(st->GetAttack());
	numDefense->SetNumber(st->GetDefense());
	numSkill->SetNumber(30);
}

//=============================================================
// テクスチャの読み込み
//=============================================================
HRESULT StatusManu::setupTextures(){

	if(FAILED(D3DXCreateTextureFromFileEx(GetDevice(), "picture/statusManu.png",800,600,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&titlePic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/iconA.png", &iconAPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), "picture/select.png", &curPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}


	return(S_OK);
}
void StatusManu::Appear(){
	title->Appear();
	cur->Appear();
	iconA->Appear();

	strHP->Appear();
	strMP->Appear();
	strEXP->Appear();
	strAttack->Appear();
	strDefense->Appear();
	strSkill->Appear();

	numHP->Appear();
	numMP->Appear();
	numEXP->Appear();
	numAttack->Appear();
	numDefense->Appear();
	numSkill->Appear();
}