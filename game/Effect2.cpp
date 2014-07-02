#include "effect2.h"

Effect::Effect()
:pFX(NULL)
{

}
Effect::~Effect(){
	SAFE_RELEASE(pFX);
}
HRESULT Effect::InitEffect(LPDIRECT3DDEVICE9 device,const EffectInfo* info){


	this->info = *info;
	pDevice = device;
	//エフェクトファイルのコンパイル
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}

	//ハンドルの取得
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}


	return S_OK;
}

HRESULT Effect::EffectCompile(){

	LPD3DXBUFFER err=NULL;
	if(FAILED(D3DXCreateEffectFromFile(pDevice,info.fileName.c_str(),NULL,NULL,D3DXSHADER_DEBUG,NULL,&pFX,&err))){
		// コンパイルエラーの場合はエラーを表示して終了
		if (!pFX) {
			MessageBox(
				NULL, 
				err?(LPCTSTR)err->GetBufferPointer():NULL, 
				"エフェクトファイルの読込エラー", MB_OK);
		}
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Effect::HandleSetting(){

	// シェーダプログラムへテクニックへのハンドルの取得
	hTech = pFX->GetTechniqueByName(info.techName.c_str());

	for(size_t i=0; i < info.valueName.size(); ++i){
		hValue.insert( make_pair(info.valueName[i].c_str() ,pFX->GetParameterByName(0,info.valueName[i].c_str() )));
	}

	//きちんとハンドルが取得できているかチェック
	map<string,D3DXHANDLE>::iterator p;
	for(p = hValue.begin(); p != hValue.end();++p){
		if(p->second == NULL){
			return E_FAIL;
		}
	}
	return S_OK;
}
HRESULT Effect::Reset(){

	hValue.clear();

	//エフェクトファイルの再コンパイル
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}
	//ハンドルの再取得
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}

	return S_OK;
}

LPD3DXEFFECT Effect::GetEffect(){
	return pFX;
}

HRESULT Effect::SetMatrix(const D3DXMATRIX* mat, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetMatrix( p->second , mat))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetVector(const D3DXVECTOR4* vec, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetVector( p->second , vec))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetTexture(p->second, tex))){
		return E_FAIL;
	}
	return S_OK;
		
}
HRESULT Effect::SetTechnique(){

	if(FAILED(pFX->SetTechnique(hTech))){
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Effect::InitSurface(){

	// シャドウマップとして使用するテクスチャの作成
	if( FAILED(pDevice->CreateTexture(
						640, 480,			
						1,	
         				D3DUSAGE_RENDERTARGET,
						D3DFMT_A8R8G8B8,	// フローティングバッファを作成
         				D3DPOOL_DEFAULT,
         				&pTexture0,
						NULL))) {	
		return E_FAIL;
	}

	// テクスチャからレンダリングターゲットにするサーフェースの取得
	if( FAILED(pTexture0->GetSurfaceLevel(0, &pTexSurface0))){
		return E_FAIL;
	}

	// テクスチャへのレンダリングに使う深度バッファーの作成
	if (FAILED(pDevice->CreateDepthStencilSurface(
						640, 480,		
						D3DFMT_D24X8,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&pTexZ0,
						NULL))){
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Effect::BeginToggleBackBuffer(){

	//========================================
	// 準備：バックバッファーの退避
	//========================================

	// ビューポート情報の保持
	pDevice->GetViewport(&viewport1);

	// バックバッファーへのインターフェースの保持
	pDevice->GetRenderTarget(0, &pBackbuffer);

	// 深度バッファーのインターフェースの保持
    pDevice->GetDepthStencilSurface( &pBackZ);

	//========================================
	//パス１：シーンの深度だけを描画
	//========================================

	// テクスチャをレンダリングターゲットに設定
	pDevice->SetRenderTarget(0, pTexSurface0);

	// テクスチャ用の深度バッファを設定
	pDevice->SetDepthStencilSurface(pTexZ0);

	// レンダリングターゲットを初期化
	if(FAILED(pDevice->Clear(0,NULL, 
								D3DCLEAR_ZBUFFER, 
								D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
								1.0f, 0))) {
		return E_FAIL;
	}


	float light_x, light_y, light_z;

	light_x = 0.5f;
	light_y = 1.0f; 
	light_z = -2.0f;


	// ビューポートをテクスチャ画像のサイズに設定
	D3DVIEWPORT9 viewport2 = { 0,0,640,480, 0.0f, 1.0f};
	pDevice->SetViewport(&viewport2);

	// アスペクト比の計算
	float aspect;
	aspect = (float)viewport2.Width / (float)viewport2.Height;


	D3DXMATRIXA16 proj;
	D3DXMATRIXA16 view;
	// プロジェクションマトリックスの作成と設定
	D3DXMatrixIdentity(&proj);
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45.0f), aspect, 1.0f, 100.0f);

	// 座標変換（光源をカメラと見立てる）
	D3DXMatrixIdentity(&view);
	D3DXMatrixLookAtLH(&view, 
						&D3DXVECTOR3(light_x, light_y, light_z), 
						&D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 
						&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	
	// テクニックの設定（シェーダプログラムの設定）
	if(FAILED(SetTechnique())){
		return E_FAIL;
	}

	// シェーダーのグローバル変数の値の設定
	if(FAILED(SetMatrix(&(view*proj),"g_vlp"))){
		return E_FAIL;
	}
	pFX->CommitChanges();

	// シェーダプログラムの開始宣言
	pFX->Begin(0, 0);
	pFX->BeginPass(0);
	// シーンを描画する

	return S_OK;

}
HRESULT Effect::EndToggleBackBuffer(){

	pFX->EndPass();
	pFX->End();	

	//==========================================
	//ターゲットを戻す
	//==========================================

	// 保存しておいたバックバッファをレンダリングターゲットに設定
	pDevice->SetRenderTarget(0, pBackbuffer);

	// 保存しておいたバックバッファの深度バッファを設定
	pDevice->SetDepthStencilSurface(pBackZ);

	// 保存しておいたビューポートを元に戻す
	pDevice->SetViewport(&viewport1);


	return S_OK;

}