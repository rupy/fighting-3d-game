#include "NumberObject2.h"
#include <sstream>
#include <iomanip>


NumberObject::NumberObject(int fontSize,
						   int fontFamily,
						   int letterSpacing,
						   D3DXVECTOR3 position,
						   D3DXCOLOR color)
						   :MySpriteObject(fontSize,fontSize,CPF_LEFT_TOP,0,position),
						   fontSize(fontSize),fontFamily(fontFamily),letterSpacing(letterSpacing),fontColor(color)
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

	decimalPlace = 0;
}

NumberObject::~NumberObject(){
}

void NumberObject::SetNumber(int number){
	this->uNumber.iNum = number;
	decimalPlace = 0;
}
void NumberObject::SetNumber(float number,int decimalPlace){
	this->uNumber.dNum = number;
	this->decimalPlace = decimalPlace;
}
void NumberObject::SetNumber(double number,int decimalPlace){
	this->uNumber.dNum = number;
	this->decimalPlace = decimalPlace;
}
void NumberObject::Appear(){

	GetDevice()->SetVertexShader(NULL);
	
	GetDevice()->SetStreamSource( 0, pMyVB, 0, sizeof(SPRITE_VERTEX) );

	GetDevice()->SetFVF( SPRITE_VERTEX_FVF );

	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE);
	GetDevice()->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_SELECTARG2 );
	D3DMATERIAL9 material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse.a = fontColor.a;
	material.Diffuse.r = fontColor.r;
	material.Diffuse.g = fontColor.g;
	material.Diffuse.b = fontColor.b;
	material.Ambient = material.Diffuse;
	material.Emissive = material.Diffuse;
	GetDevice()->SetMaterial(&material);

	basePos = Pos;//元の位置を保存しておく

	ostringstream oss;
	if(decimalPlace == 0){
		oss<<setprecision(0)<<uNumber.iNum;
	}else{
		oss<<setprecision(decimalPlace)<<fixed;
		oss<<uNumber.dNum;
	}
	char* strNum = new char[oss.str().length()+1];
	strncpy(strNum,oss.str().c_str(),oss.str().length()+1);


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
		Scale.x = font[i].fTexW;
		Scale.y = font[i].fTexH;
		D3DXMATRIXA16 matWorld = MatrixSetting();
		GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);


		GetDevice()->SetTexture(0,pTex = font[i].pTex);
		GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		Pos.x += Scale.x + letterSpacing;//次の位置へシフト
	}
	Pos = basePos;//位置を戻す
	delete[] strNum;

	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);
	GetDevice()->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_BLENDTEXTUREALPHA );
}