#include "StringObject.h"

StringObject_S::StringObject_S(LPD3DXSPRITE sprite,
						   D3DXVECTOR3 pos,
						   float rightEdge,
						   int fontSize,
						   int fontFamily,
						   D3DXCOLOR color,
						   bool changeLine,
						   float indent,
						   float lineHeight,
						   float letterSpacing,
						   int speed,
						   TCHAR t[])
{
	this->color = D3DCOLOR_ARGB(255,255,255,255);
	this->fontSize = fontSize;
	this->fontFamily = fontFamily;
	this->changeLine = changeLine;
	this->indent = indent;
	this->lineHeight = lineHeight;
	this->letterSpacing = letterSpacing;
	this->rightEdge = rightEdge;

	this->pSprite = sprite;

	this->Pos = pos;

//	vecCenter = D3DXVECTOR3(0,0,0);

	

//	TCHAR t[] = *str;
//	int a = MultiByteStringLength(t);
	size_t i=0;
	while(t[i]){
		font.push_back(Font());

		if(t[i]=='<'){//'<'が登場したらサイズ変更の可能性がある
			i+=SetSize(&t[i]);
		}

		font.back().MakeFont(GetDevice(),&t[i],this->fontSize);
		if(IsDBCSLeadByte(t[i])){
			i+=2;
		}else{
			i++;
		}	
	}

	uvRect.top = uvRect.left = uvRect.right = uvRect.bottom = 0;

	this->speed = speed;
	cur = 0;
	maxLine = 0;
	time = 0;

	displayEnd = 0;
}
StringObject_S::~StringObject_S(){}


int StringObject_S::SetSize(TCHAR *c){
	int k=0;
	if((strncmp(c,"<size=",6))==NULL){
		if(char* e = strchr(c,'>')){
			k=1+(unsigned)(e-c);
		}
		c+=6;
		fontSize = atoi(c);
	}
	return k;
}

void StringObject_S::Appear(){

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	pSprite->SetTransform(&mat);

	uvRect.top=0;
	uvRect.left=0;
	uvRect.right=0;
	uvRect.bottom=0;

	
	curPos=Pos;//カーソールを初期値に合わせる

	curPos.x+=indent*fontSize;


	if(cur != font.size()){//カーソールが末尾までいっていなければ
		if(!speed){//スピード0の時
			cur = font.size();//一度に表示
		}else{//0以外
			time++;//時間経過
				if(time>=speed){//時間がspeedに達したら
					cur++;//カーソール移動
					time = 0;//時間を0に
				}
		}
	}else{
		displayEnd=1;
	}

	pSprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_DONOTSAVESTATE);


	for(int i=0;i<cur;i++){//カーソール位置まで描画する

		if(!font[i].pTex){//pTexがNULLの場合改行
				curPos.y += maxLine+lineHeight;
				curPos.x = Pos.x;
				continue;
		}

		this->uvRect.right = (LONG)font[i].fTexW;
		this->uvRect.bottom = (LONG)font[i].fTexH;
		
		if(changeLine){
			//右端に来たら改行
			if(curPos.x>rightEdge-uvRect.right){
				curPos.y += maxLine+lineHeight;
				curPos.x = Pos.x;
			}
		}

		//行の最大幅を保存する
		if(maxLine<uvRect.bottom-uvRect.top){
			maxLine = uvRect.bottom-uvRect.top;
		}

		D3DXVECTOR3 vecCenter = D3DXVECTOR3(0,0,0);

		//描画
		if(FAILED(pSprite->Draw(font[i].pTex,
			&uvRect,
			&vecCenter,
			&curPos,
			color))){
				//return(E_FAIL);
				
		}
		
		curPos.x += uvRect.right + letterSpacing;//次の位置へシフト
	}


	pSprite->End();
}

int MultiByteStringLength(TCHAR st[]){
	int i=0;
	int n=0;
	while(st[i]){
		n++;
		if(IsDBCSLeadByte(st[i])){
			i+=2;
		}else{
			i++;
		}
	}
	return n;
}
