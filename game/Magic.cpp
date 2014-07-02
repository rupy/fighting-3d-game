#include "Magic.h"

Magic::Magic(vector<Effect*>& eff,
		int time)
:count(50),mode(MAGIC_HEAL),time(0),endtime(time)
{
	if(FAILED(D3DXCreateTextureFromFile( GetDevice(),MAGIC_TEX_FILE ,&pTex))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return;
	}

	for(int i=0;i<count;++i){
		particle.push_back(new PolarCoordinates(eff,pTex));
		particle.back()->parentObj = this;
		particle.back()->theta = D3DX_PI *RAND_0_1();//角度はばらばらにしておく
		Initialize(i);
	}

}

Magic::~Magic()
{
	for(int i=0;i<count;++i){
		SAFE_DELETE(particle[i]);
	}
	SAFE_RELEASE(pTex);
}
void Magic::Initialize(){
	for(int i=0;i<count;++i){
		Initialize(i);
	}
}
void Magic::Initialize(int idx)
{
	switch(mode){
		case MAGIC_HEAL:
			particle[idx]->radius = info.start_rad;
			particle[idx]->height = info.height_min;
			//速さの指定
			particle[idx]->v_radius = info.rad_min_speed + (info.rad_max_speed - info.rad_min_speed) * RAND_0_1();
			particle[idx]->v_height = info.height_min_speed + (info.height_max_speed - info.rad_min_speed) * RAND_0_1();
			particle[idx]->v_theta = info.theta_min_speed + (info.theta_max_speed - info.theta_min_speed) * RAND_0_1();
			particle[idx]->Scale = D3DXVECTOR3(1.0f,1.0f,1.0f) * (info.size_min + (info.size_max - info.size_min) * RAND_0_1());
			break;
		case MAGIC_IN:
			particle[idx]->radius = info.start_rad;
			particle[idx]->height = info.height_max;
			//速さの指定
			particle[idx]->v_radius = info.rad_min_speed + (info.rad_max_speed - info.rad_min_speed) * RAND_0_1();
			particle[idx]->v_height = info.height_min_speed + (info.height_max_speed - info.rad_min_speed) * RAND_0_1();
			particle[idx]->v_theta = info.theta_min_speed + (info.theta_max_speed - info.theta_min_speed) * RAND_0_1();
			particle[idx]->Scale = D3DXVECTOR3(1.0f,1.0f,1.0f) * (info.size_min + (info.size_max - info.size_min) * RAND_0_1());
			break;
	}

}


bool Magic::Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){
	time++;

	for(int i=0;i<count;++i){
		particle[i]->LookAtMe(look_pos,cam_pos);

		switch(mode){
			case MAGIC_HEAL:
				particle[i]->radius += particle[i]->v_radius;
				particle[i]->theta += particle[i]->v_theta;
				particle[i]->height += particle[i]->v_height;
				if(particle[i]->height > info.height_max){
					Initialize(i);
				}
				break;
			case MAGIC_IN:
				particle[i]->radius -= particle[i]->v_radius;
				particle[i]->theta -= particle[i]->v_theta;
				particle[i]->height -= particle[i]->v_height;
				if(particle[i]->height < info.height_min){
					Initialize(i);
				}
				break;
		}

	}
	if(time > endtime){
		return true;
	}
	return false;
}
void Magic::Appear(){

	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_MODULATE );

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	GetDevice()->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
	GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
	GetDevice()->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_MODULATE );



	for(int i=0;i<count;++i){

		D3DMATERIAL9 mat;
		ZeroMemory(&mat,sizeof(mat));
		mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(
			info.color_red,info.color_green,info.color_blue,
			sin(D3DX_PI*(float)particle[i]->height/info.height_max) * sin(D3DX_PI*(float)time/endtime)
			);
		GetDevice()->SetMaterial(&mat);

		particle[i]->Appear();
	}

	//リセット
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
}
