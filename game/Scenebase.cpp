#include "Scenebase.h"
#include "Common.h"

SceneBase::SceneBase(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm)
:pDevice(device),pSprite(sprite),pSound(sound),pSM(sm),
light(NULL),canClear(true),sceneMode(0)
{
	camera.push_back(new Camera(eff));
}
SceneBase::~SceneBase(){
	SAFE_DELETE(light);

	for(size_t i=0;i<eff.size();++i){
		SAFE_DELETE(eff[i]);
	}
	for(size_t i=0;i<target.size();++i){
		SAFE_DELETE(target[i]);
	}
	for(size_t i=0;i<camera.size();++i){
		SAFE_DELETE(camera[i]);
	}
}
void SceneBase::ClearFor2DDraw(){
	//深度のクリア
	pDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 40, 40, 80 ), 1.0f, 0 );
	
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	//2D用のビュー行列、射影行列の設定
	D3DXMATRIX matView, matProj;
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);
	pDevice->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixOrthoOffCenterLH( &matProj, 0,BASE_WIDTH,0, BASE_HEIGHT, 0.0f, 1.0f ); 
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	SetRenderStateFor2D();

}

void SceneBase::SetRenderStateFor2D(){

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	
	pDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	pDevice->SetTextureStageState( 0 , D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE );
	pDevice->SetTextureStageState( 0 , D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );
	
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2,	D3DTA_DIFFUSE );
	pDevice->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_BLENDTEXTUREALPHA );

	pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
}