#include "Fire.h"

FireParticle::FireParticle(DummyObject* parent,bool tail)
{
	parentObj = parent;
	if(tail){
		validParent = false;
	}
	Initialize();

	//そろわないようにはじめ少し進めておく
	time = (int)(endtime * RAND_0_1());
	Pos = (vPos * (float)time);
}
void FireParticle::Initialize(){

	Pos = validParent?D3DXVECTOR3(0,0,0):parentObj->Pos;
	Scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	float rad = RAND_0_1()*D3DX_PI*2;
	float spread = RAND_05()*FIRE_WIDESPREAD;
	Pos += D3DXVECTOR3(cos(rad)*spread,0,sin(rad)*spread);
	vPos = D3DXVECTOR3(0,FIRE_MIN_SPEED+(FIRE_MAX_SPEED-FIRE_MIN_SPEED)*RAND_0_1(),0);
	Scale *= FIRE_MIN_SIZE+(FIRE_MAX_SIZE-FIRE_MIN_SIZE)*RAND_0_1();
	time = 0;
	endtime = (int)(FIRE_MIN_TIME + (FIRE_MAX_TIME - FIRE_MIN_TIME) * RAND_0_1());
}
void FireParticle::Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){

	D3DXVECTOR3 dir = *look_pos - *cam_pos;
	this->lookatPt = dir;
	this->upVec    =D3DXVECTOR3(0,1.0f,0);

	Pos += vPos;
	time++;

	if(endtime < time){
		Initialize();
	}
	D3DXVECTOR3 dirCamVec = Pos - *cam_pos;
	dirCam = D3DXVec3Length(&dirCamVec);
}

void FireParticle::Appear(){

	D3DXMATRIXA16 matWorld = BillboardMatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);

	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(FIRE_COLOR_RED,FIRE_COLOR_GREEN,FIRE_COLOR_BLUE,sin(D3DX_PI*(float)(endtime - time)/endtime));
	GetDevice()->SetMaterial(&mat);

	//この中では描画は行っていない。実際にはFireクラス内で描画

}

Fire::Fire(int fire_count,bool tail,D3DXVECTOR3 pos)
:firePic(NULL),tail(tail),DummyObject(pos)
{
	particleVertex[0].Tex = D3DXVECTOR2(0,0);
	particleVertex[1].Tex = D3DXVECTOR2(1,0);
	particleVertex[2].Tex = D3DXVECTOR2(0,1);
	particleVertex[3].Tex = D3DXVECTOR2(1,1);

    particleVertex[0].Pos = D3DXVECTOR3(-0.5f, 0.5f, 0.0f );
    particleVertex[1].Pos = D3DXVECTOR3( 0.5f, 0.5f, 0.0f );
    particleVertex[2].Pos = D3DXVECTOR3(-0.5f, -0.5f,0.0f );
    particleVertex[3].Pos = D3DXVECTOR3( 0.5f, -0.5f,0.0f );
	SetupTextures();

	for(int i=0;i<fire_count;++i){
		fire_particle.push_back(FireParticle(this,tail));
	}
}

Fire::~Fire(){
	SAFE_RELEASE(firePic);
}
/*void Rain::Add(int add_count){
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
}*/
void Fire::Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){
	int fire_count = fire_particle.size();
	for(int i=0;i<fire_count;++i){
		fire_particle[i].Move(look_pos,cam_pos);
	}
}

void Fire::Appear(){

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_FIRE_VERTEX);

	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_MODULATE );

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	//GetDevice()->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
	GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	GetDevice()->SetTexture(0,firePic);

	int fire_count = fire_particle.size();
	for(int i=0;i<fire_count;++i){
		fire_particle[i].Appear();
		GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,particleVertex,sizeof(FireVertex));
	}

	//リセット
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
}
HRESULT Fire::SetupTextures(){

	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), FIREPARTICLE_TEX_FILE ,&firePic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	return S_OK;
}
