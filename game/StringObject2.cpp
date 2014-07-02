#include "StringObject2.h"

StringObject::StringObject(D3DXVECTOR3 pos,
						   float rightEdge,
						   int fontSize,
						   int fontFamily,
						   D3DXCOLOR color,
						   bool changeLine,
						   float indent,
						   float lineHeight,
						   float letterSpacing,
						   int speed,
						   const TCHAR t[])
						   :MySpriteObject(1,1,CPF_LEFT_TOP,0,pos),nextaction(0)
{
	this->fontSize = fontSize;
	this->fontFamily = fontFamily;
	this->changeLine = changeLine;
	this->indent = indent;
	this->lineHeight = lineHeight;
	this->letterSpacing = letterSpacing;
	this->rightEdge = rightEdge;
	this->fontColor = color;


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


	this->speed = speed;
	cur = 0;
	maxLine = 0;
	time = 0;

	displayEnd = 0;
}
StringObject::~StringObject(){}


int StringObject::SetSize(const TCHAR *c){
	int k=0;
	if((strncmp(c,"<size=",6))==NULL){
		if(const char* e = strchr(c,'>')){
			k=1+(unsigned)(e-c);
		}
		c+=6;
		fontSize = atoi(c);
	}
	return k;
}

void StringObject::Appear(){

	
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

	basePos = Pos;
	
	basePos=Pos;//�J�[�\�[���������l�ɍ��킹��

	Pos.x+=indent*fontSize;


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

	for(int i=0;i<cur;i++){//�J�[�\�[���ʒu�܂ŕ`�悷��

		if(!font[i].pTex){//pTex��NULL�̏ꍇ���s
				Pos.y -= maxLine+lineHeight;
				Pos.x = basePos.x;
				continue;
		}

		Scale.x = font[i].fTexW;
		Scale.y = font[i].fTexH;
		
		if(changeLine){
			//�E�[�ɗ�������s
			if(Pos.x>rightEdge-Scale.x){
				Pos.y += maxLine+lineHeight;
				Pos.x = basePos.x;
			}
		}

		//�s�̍ő啝��ۑ�����
		if(maxLine<Scale.y){
			maxLine = Scale.y;
		}

		D3DXMATRIXA16 matWorld = MatrixSetting();
		GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);


		GetDevice()->SetTexture(0,pTex = font[i].pTex);
		GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

		Pos.x += Scale.x + letterSpacing;//���̈ʒu�փV�t�g
	}
	endPos= Pos;
	Pos = basePos;


	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);
	GetDevice()->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_BLENDTEXTUREALPHA );
}