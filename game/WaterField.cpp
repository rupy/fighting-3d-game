#include "WaterField.h"
#include "Common.h"

WaterField::WaterField(
		vector<Effect*>& eff,
		D3DXVECTOR3 pos)
:water(eff),Object(pos),size(10),scale(1000),time(0),speed(30000),wavetime(0),wavespeed(300)
{
	water.Roll = D3DXToRadian(90.0f);
	water.Scale *= scale;
	water.alpha = 0.8f;
	SetupTexture();
	Pos += D3DXVECTOR3(-(float)size*scale/2,0,-(float)size*scale/2);
}
WaterField::~WaterField(){}

void WaterField::Move(){

	time++;wavetime++;
	if(time>speed){
		time =0;
	}
	if(wavetime>wavespeed){
		wavetime=0;
	}

	//頂点バッファーの中身を埋める
	VertexObject::MY_VERTEX* v;
	water.pMyVB->Lock( 0, 0, (void**)&v, 0 );

	//頂点データ：頂点の座標値の設定
    v[0].tu = 0.0f + (float)time/speed/2;	// 頂点①
    v[0].tv = 0.0f + (float)time/speed/2;	// 頂点①
    v[1].tu = 0.5f + (float)time/speed/2;	// 頂点②
    v[1].tv = 0.0f + (float)time/speed/2;	// 頂点②
    v[2].tu = 0.0f + (float)time/speed/2;	// 頂点③
    v[2].tv = 0.5f + (float)time/speed/2;	// 頂点③
    v[3].tu = 0.5f + (float)time/speed/2;	// 頂点④
    v[3].tv = 0.5f + (float)time/speed/2;	// 頂点④

	v[0].p.z = 0;	// 頂点①
	v[1].p.z = 0;	// 頂点②
	v[2].p.z = 0;	// 頂点③
    v[3].p.z = 0;	// 頂点④


	water.pMyVB->Unlock();

	Pos.y =sin((float)wavetime/wavespeed*2*D3DX_PI)*0.3f;

	
	//MyOutputDebugString("<%f>\n",(float)wavetime);
}
void WaterField::Appear(){
	
	GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );

	GetDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(255,255,255,0);
	GetDevice()->SetMaterial(&mat);

	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	water.Pos = Pos;
	for(int i=0;i<size;++i){
		water.Pos.x = Pos.x;
		for(int j=0;j<size;++j){
			water.Appear();
			water.Pos.x += scale;
		}
		water.Pos.z += scale;
	}
	GetDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
}
HRESULT WaterField::SetupTexture(){
	if(FAILED(water.SetupTexture(SEA_TEXTURE_FILE))){
		return E_FAIL;
	}
	return S_OK;
}