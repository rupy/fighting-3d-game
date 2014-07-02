#include "Rain.h"

RainDrop::RainDrop(float start_height)
:startHeight(start_height),state(0),time(0)
{

	dropVertex[0].Pos = D3DXVECTOR3(0,0.5f,0);
	dropVertex[1].Pos = D3DXVECTOR3(0,-0.5f,0);
	dropVertex[0].Color=dropVertex[1].Color=D3DXCOLOR(1,1,1,1);

	for(int i=0;i<RIPPLE_VERTEX_COUNT;++i){
		float rad=D3DX_PI*2*i/(RIPPLE_VERTEX_COUNT-1);
		rippleVertex[i].Pos=D3DXVECTOR3(cos(rad),0,sin(rad));
		rippleVertex[i].Color=D3DXCOLOR(1,0,1,1);
//		MyOutputDebugString("[%f,%f,%f]%f\n",rippleVertex[i].Pos.x,rippleVertex[i].Pos.y,rippleVertex[i].Pos.z,rad);
	}

	Initialize();
}
RainDrop::RainDrop(const RainDrop& rain_drop)
:startHeight(rain_drop.startHeight),state(0),time(0)
{

	dropVertex[0].Pos = D3DXVECTOR3(0,0.5f,0);
	dropVertex[1].Pos = D3DXVECTOR3(0,-0.5f,0);
	dropVertex[0].Color=dropVertex[1].Color=D3DXCOLOR(1,1,1,1);

	for(int i=0;i<RIPPLE_VERTEX_COUNT;++i){
		float rad=D3DX_PI*2*i/(RIPPLE_VERTEX_COUNT-1);
		rippleVertex[i].Pos=D3DXVECTOR3(cos(rad),0,sin(rad));
		rippleVertex[i].Color=D3DXCOLOR(1,0,1,1);
//		MyOutputDebugString("[%f,%f,%f]%f\n",rippleVertex[i].Pos.x,rippleVertex[i].Pos.y,rippleVertex[i].Pos.z,rad);
	}

	Initialize();
}
RainDrop::~RainDrop(){
}
void RainDrop::Initialize(){
	Pos = D3DXVECTOR3(RAND_05()*20.0f,RAND_0_1()*startHeight,RAND_05()*20.0f);
	vPos = D3DXVECTOR3(0,-0.5f,0);
	aPos = D3DXVECTOR3(0,0,0);

	Scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	state=0;
}
void RainDrop::Move(){

	if(state==0){
		Pos+=vPos;
		if(Pos.y<0){
			time=0;
			rippleTime=(int)(RAND_0_1()*20);
			state=1;
			Scale = D3DXVECTOR3(0,0,0);
		}
	}else{
		time++;
		Scale=D3DXVECTOR3(1.0f,1.0f,1.0f)*(float)time*0.01f;
		Pos.y = .1f;
		if(time>rippleTime){
			time=0;
			Initialize();
		}
	}
}
void RainDrop::Appear(){


	D3DXMATRIXA16 matWorld = MatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);


	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	if(state==0){
		mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(.8f,.8f,1.0f,sin(D3DX_PI*(startHeight-Pos.y)/startHeight));
		GetDevice()->SetMaterial(&mat);
		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,dropVertex,sizeof(RainVertex));
	}else{
		mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(.8f,.8f,1.0f,(float)(rippleTime-time)/rippleTime);
		GetDevice()->SetMaterial(&mat);
		GetDevice()->DrawPrimitiveUP(D3DPT_LINESTRIP,RIPPLE_VERTEX_COUNT-1,rippleVertex,sizeof(RainVertex));
	}
}

Rain::Rain(float start_height,int rain_count)
{
	for(int i=0;i<rain_count;++i){
		rain_drops.push_back(RainDrop(start_height));
	}
}

Rain::~Rain(){
}
void Rain::Add(int add_count){
	for(int i=0;i<add_count;++i){
		rain_drops.push_back(rain_drops.back());
	}
}
void Rain::Reduce(int reduce_count){
	for(int i=0;i<reduce_count;++i){
		if(rain_drops.size()!=0){
			rain_drops.pop_back();
		}
	}
}
void Rain::Move(){
	int rain_count = rain_drops.size();
	for(int i=0;i<rain_count-1;++i){
		rain_drops[i].Move();
	}
}

void Rain::Appear(){
	
	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_RAIN_VERTEX);

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	int rain_count = rain_drops.size();
	for(int i=0;i<rain_count-1;++i){
		rain_drops[i].Appear();
	}

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
}