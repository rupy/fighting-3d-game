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

		if(t[i]=='<'){//'<'���o�ꂵ����T�C�Y�ύX�̉\��������
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

	
	curPos=Pos;//�J�[�\�[���������l�ɍ��킹��

	curPos.x+=indent*fontSize;


	if(cur != font.size()){//�J�[�\�[���������܂ł����Ă��Ȃ����
		if(!speed){//�X�s�[�h0�̎�
			cur = font.size();//��x�ɕ\��
		}else{//0�ȊO
			time++;//���Ԍo��
				if(time>=speed){//���Ԃ�speed�ɒB������
					cur++;//�J�[�\�[���ړ�
					time = 0;//���Ԃ�0��
				}
		}
	}else{
		displayEnd=1;
	}

	pSprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_DONOTSAVESTATE);


	for(int i=0;i<cur;i++){//�J�[�\�[���ʒu�܂ŕ`�悷��

		if(!font[i].pTex){//pTex��NULL�̏ꍇ���s
				curPos.y += maxLine+lineHeight;
				curPos.x = Pos.x;
				continue;
		}

		this->uvRect.right = (LONG)font[i].fTexW;
		this->uvRect.bottom = (LONG)font[i].fTexH;
		
		if(changeLine){
			//�E�[�ɗ�������s
			if(curPos.x>rightEdge-uvRect.right){
				curPos.y += maxLine+lineHeight;
				curPos.x = Pos.x;
			}
		}

		//�s�̍ő啝��ۑ�����
		if(maxLine<uvRect.bottom-uvRect.top){
			maxLine = uvRect.bottom-uvRect.top;
		}

		D3DXVECTOR3 vecCenter = D3DXVECTOR3(0,0,0);

		//�`��
		if(FAILED(pSprite->Draw(font[i].pTex,
			&uvRect,
			&vecCenter,
			&curPos,
			color))){
				//return(E_FAIL);
				
		}
		
		curPos.x += uvRect.right + letterSpacing;//���̈ʒu�փV�t�g
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
