#include "NumberObject.h"
#include <sstream>
#include <iomanip>


NumberObject2::NumberObject2(LPD3DXSPRITE sprite,
						   int fontSize,
						   int fontFamily,
						   int letterSpacing,
						   D3DXVECTOR3 position,
						   D3DCOLOR color)
						   :Object(position,0.0f,0.0f,0.0f),pSprite(sprite),
						   fontSize(fontSize),fontFamily(fontFamily),letterSpacing(letterSpacing)
{
	//0から9
	for(size_t i=0;i<10;i++){
		char char_num = '0' + i;
		font.push_back(Font());
		font.back().MakeFont(GetDevice(),&char_num,this->fontSize);
	}

	//マイナス記号
	char minus = '-';
	font.push_back(Font());
	font.back().MakeFont(GetDevice(),&minus,this->fontSize);

	//小数点
	char dot = '.';
	font.push_back(Font());
	font.back().MakeFont(GetDevice(),&dot,this->fontSize);

	uvRect.bottom = uvRect.left = uvRect.right = uvRect.top = 0;

	decimalPlace = 0;
}

NumberObject2::~NumberObject2(){
}

void NumberObject2::SetNumber(int number){
	this->uNumber.iNum = number;
	decimalPlace = 0;
}
void NumberObject2::SetNumber(float number,int decimalPlace){
	this->uNumber.dNum = number;
	this->decimalPlace = decimalPlace;
}
void NumberObject2::SetNumber(double number,int decimalPlace){
	this->uNumber.dNum = number;
	this->decimalPlace = decimalPlace;
}
void NumberObject2::Appear(){

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	pSprite->SetTransform(&mat);

	curPos=Pos;//カーソールを初期値に合わせる


	ostringstream oss;
	if(decimalPlace == 0){
		oss<<setprecision(0)<<uNumber.iNum;
	}else{
		oss<<setprecision(decimalPlace)<<fixed;
		oss<<uNumber.dNum;
	}
	char* strNum = new char[oss.str().length()+1];
	strncpy(strNum,oss.str().c_str(),oss.str().length()+1);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_DONOTSAVESTATE);

	//ナル文字がくるまでループ
	for(char* p = strNum; *p; ++p){
		char c = *p;
		int i;
		if('0'<= c && c <= '9'){
			i = atoi(&c);
		}else if(c == '-'){
			i = 10;
		}else if(c == '.'){
			i = 11;
		}else{
			i = 0;
		}

		this->uvRect.right = (LONG)font[i].fTexW;
		this->uvRect.bottom = (LONG)font[i].fTexH;

		D3DXVECTOR3 vecCenter = D3DXVECTOR3(0,0,0);

		//描画
		if(FAILED(pSprite->Draw(font[i].pTex,
			&uvRect,
			&vecCenter,
			&curPos,
			D3DCOLOR_ARGB(255,255,255,255)
			))){
				//return(E_FAIL);
				
		}
		curPos.x += uvRect.right + letterSpacing;//次の位置へシフト
	}
	pSprite->End();

	delete[] strNum;

}