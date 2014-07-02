#include "Game.h"
#include "Common.h"

LPDIRECT3DDEVICE9 ResourceBase::pDevice;
//=============================================================
// コンストラクタ
//=============================================================
GameBase::GameBase(LPDIRECT3DDEVICE9		device,
				   D3DPRESENT_PARAMETERS*	param,
				   LPD3DXSPRITE				sprite,
				   Input*					input,
				   Sound*					sound)
:pDevice(device),pD3DPP(param),pSprite(sprite),pInput(input),pSound(sound),
scene(NULL)
{
	ResourceBase::SetDevice(device);

	SM.InitState();
	scene = new StartScene(device,sprite,pSound,&SM);//はじめはシーン１
	if(FAILED(scene->Initialize())){		
		PostQuitMessage(0);
	}

}
//=============================================================
// デストラクタ
//=============================================================
GameBase::~GameBase(){

	SAFE_DELETE(scene);
}
//=============================================================
// ゲームの実行
//=============================================================
int GameBase::Run(){

	KeyMessage keyMessage = pInput->GetKeybordState();
	pInput->GetJoystickState(&keyMessage,0);
	pInput->GetCommand(&keyMessage);

	SceneBase* next=NULL;

	next = scene->Move(&keyMessage);
	if(next!=NULL){
		//新しいシーンを受け取ったらシーンを切り替えて初期化
		SAFE_DELETE(scene);
		scene = next;
		scene->Initialize();
		scene->Move(&keyMessage);//新しいシーンでの描画
	}

	return 0;
}

//=============================================================
// バックバッファへ書き込み
//=============================================================
HRESULT GameBase::Draw(){


	if(scene->canClear){
		RecetRenderState();//レンダーステートのリセット

		// ビューポートと深度バッファの初期化とステンシルバッファの削除
		if(FAILED(pDevice->Clear(0,NULL, 				// 初期化する領域は全面
			D3DCLEAR_TARGET |					// バックバッファを指定
			D3DCLEAR_ZBUFFER, 					// 深度バッファ（Zバッファ）を指定
			D3DXCOLOR(0.7f,0.9f,0.9f, 1.0f), 	// 初期化する色
			1.0f, 								// 初期化する深度バッファ（Zバッファ）の値
			0))) {								// 初期化するステンシルバッファの値
				return(E_FAIL);
		}

		// 描画開始宣言
		if(SUCCEEDED(pDevice->BeginScene())) {
			// モデルなどの描画
			scene->Draw();
			// 描画終了宣言
			pDevice->EndScene();
		}
	}//else{
	//			scene->Draw();
	//}


	return(S_OK);
}

//=============================================================
// 画面の表示
//=============================================================
HRESULT GameBase::Display()
{
	if(scene->canClear){
		// 描画結果の転送
		if(FAILED(pDevice->Present( 0, 0, 0, 0 ))) {
			// デバイス消失から復帰を試みる
			if(FAILED(DeviceRecet())){
				return E_FAIL;
			}
		}
	}

	return S_OK;
}
//=============================================================
// 基本的なレンダーステートの設定
//=============================================================
void GameBase::RecetRenderState(){
//	// 両面描画モードの指定
//	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	// Ｚ比較を行なう
//	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
//	// ディザリングを行なう（高品質描画）
//	pDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
//	// ノーライティングモード 	
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
//	// 環境光 	
//	pDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080);
//
//	// テクスチャステージステートの設定
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG1,	D3DTA_TEXTURE );	// テクスチャの色を使用
	pDevice->SetTextureStageState( 0 , D3DTSS_COLORARG2,	D3DTA_DIFFUSE );	// 頂点の色を使用
	pDevice->SetTextureStageState( 0 , D3DTSS_COLOROP,	D3DTOP_MODULATE );	// 乗算する
//
	pDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	pDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );
	pDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);

//	pDevice->LightEnable(0,FALSE);

}
//=============================================================
// デバイスのリセット
//=============================================================
HRESULT GameBase::DeviceRecet(){

	HRESULT hr;

	//=============================
	//リセット
	//=============================
	//スプライト
	pSprite->OnLostDevice();

	//エフェクト
	for(size_t i=0;i<scene->eff.size();++i){
		scene->eff[i]->pFX->OnLostDevice();
	}

	for(size_t i=0;i<scene->target.size();++i){
		scene->target[i]->ReleaseSurface();
	}

	//=============================
	//デバイスロストのチェック
	//=============================
	if(FAILED(hr = pDevice->TestCooperativeLevel())){
		if(hr == D3DERR_DEVICELOST){
			Sleep(100);								// 0.1秒待つ
			return S_OK;
		}
		if(hr != D3DERR_DEVICENOTRESET){
			return E_FAIL;
		}

		if(FAILED(hr = pDevice->Reset(pD3DPP))){
			if(hr == D3DERR_DEVICELOST){
				return S_OK;
			}
			return E_FAIL;
		}
	}

	//=============================
	//リセットと復元
	//=============================
	//デバイス
	pDevice->Reset(pD3DPP);
	//スプライト
	pSprite->OnResetDevice();
	//エフェクト
	for(size_t i=0;i<scene->eff.size();++i){
		scene->eff[i]->pFX->OnResetDevice();
	}
	for(size_t i=0;i<scene->target.size();++i){
		scene->target[i]->ResetSurface();
	}
	//RecetRenderState();

	return S_OK;
}