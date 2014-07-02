#include "Snow.h"

SnowBall::SnowBall(float start_height)
:startHeight(start_height),dirCam(0)
{
	Initialize();
	
	Pos = D3DXVECTOR3(RAND_05()*20.0f,RAND_0_1()*startHeight,RAND_05()*20.0f);
}
void SnowBall::Initialize(){

	Pos = D3DXVECTOR3(RAND_05()*20.0f,startHeight,RAND_05()*20.0f);
	Scale = D3DXVECTOR3(0.3f,0.3f,0.3f);
}
void SnowBall::Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){

	D3DXVECTOR3 dir = *look_pos - *cam_pos;
	this->lookatPt = dir;
	this->upVec    =D3DXVECTOR3(0,1.0f,0);

	Pos.y += SNOW_SPEED;

	if(Pos.y < 0){
		Initialize();
	}
	
	D3DXVECTOR3 dirCamVec = Pos - *cam_pos;
	dirCam = D3DXVec3Length(&dirCamVec);
}

void SnowBall::Appear(){

	D3DXMATRIXA16 matWorld = BillboardMatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);

	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(1.0f,1.0f,1.0f,sin(D3DX_PI*(startHeight-Pos.y)/startHeight));
	GetDevice()->SetMaterial(&mat);

	//この中では描画は行っていない。実際にはSnowクラス内で描画

}
bool compare(const SnowBall& a,const SnowBall& b){
	//MyOutputDebugString("%06f/%06f\n",a.dirCam,b.dirCam);
	return (a.dirCam > b.dirCam);
}
Snow::Snow(float start_height,int snow_count)
:snowPic(NULL)
{
	ballVertex[0].Tex = D3DXVECTOR2(0,0);
	ballVertex[1].Tex = D3DXVECTOR2(1,0);
	ballVertex[2].Tex = D3DXVECTOR2(0,1);
	ballVertex[3].Tex = D3DXVECTOR2(1,1);

    ballVertex[0].Pos = D3DXVECTOR3(-0.5f, 0.5f, 0.0f );
    ballVertex[1].Pos = D3DXVECTOR3( 0.5f, 0.5f, 0.0f );
    ballVertex[2].Pos = D3DXVECTOR3(-0.5f, -0.5f,0.0f );
    ballVertex[3].Pos = D3DXVECTOR3( 0.5f, -0.5f,0.0f );
	SetupTextures();

	for(int i=0;i<snow_count;++i){
		snow_ball.push_back(SnowBall(start_height));
	}
}

Snow::~Snow(){
}
void Snow::Add(int add_count){
	for(int i=0;i<add_count;++i){
		snow_ball.push_back(snow_ball.back());
	}
}
void Snow::Reduce(int reduce_count){
	for(int i=0;i<reduce_count;++i){
		if(snow_ball.size()!=0){
			snow_ball.pop_back();
		}
	}
}
void Snow::Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos){

	int snow_count = snow_ball.size();
	for(int i=0;i<snow_count;++i){
		snow_ball[i].Move(look_pos,cam_pos);
	}

}

void Snow::Appear(){

	sort(snow_ball.begin(),snow_ball.end(),compare);
	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_SNOW_VERTEX);

	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	GetDevice()->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_MODULATE );

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//GetDevice()->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);

	GetDevice()->SetTexture(0,snowPic);

	int snow_count = snow_ball.size();
	for(int i=0;i<snow_count;++i){
		snow_ball[i].Appear();
		GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,ballVertex,sizeof(SnowVertex));
	}
	//リセット
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
}
HRESULT Snow::SetupTextures(){

	if(FAILED(D3DXCreateTextureFromFile( GetDevice(), SNOWBALL_TEX_FILE ,&snowPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	return S_OK;
}
