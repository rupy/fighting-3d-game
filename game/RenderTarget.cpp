#include "RenderTarget.h"
#include "Common.h"
#include <dxerr.h>
#include "Error.h"

RenderTarget::RenderTarget(LPDIRECT3DDEVICE9 device)
:width(0),height(0),
pBackbuffer(NULL),pBackZ(NULL),
pTexture0(NULL),pTexSurface0(NULL),pTexZ0(NULL)
{
}
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(pBackbuffer);
	SAFE_RELEASE(pBackZ);
	ReleaseSurface();
}

HRESULT RenderTarget::InitSurface(int width,int height){

	this->width = width;
	this->height = height;
	HRESULT hr;
	// シャドウマップとして使用するテクスチャの作成
	if( FAILED(GetDevice()->CreateTexture(
		width, height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A32B32G32R32F,	// フローティングバッファを作成		
		D3DPOOL_DEFAULT,
		&pTexture0,
		NULL))) {
			if( FAILED(hr = GetDevice()->CreateTexture(
				width, height,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_R16F,
				D3DPOOL_DEFAULT,
				&pTexture0,
				NULL))) {
					if( FAILED(hr = GetDevice()->CreateTexture(
						width, height,
						1,
						D3DUSAGE_RENDERTARGET,
						D3DFMT_G16R16F,
						D3DPOOL_DEFAULT,
						&pTexture0,
						NULL))) {
							if( FAILED(hr = GetDevice()->CreateTexture(
								width, height,
								1,
								D3DUSAGE_RENDERTARGET,
								D3DFMT_G16R16,
								D3DPOOL_DEFAULT,
								&pTexture0,
								NULL))) {
									if( FAILED(hr = GetDevice()->CreateTexture(
										width, height,
										1,	
										D3DUSAGE_RENDERTARGET,
										D3DFMT_A16B16G16R16,
										D3DPOOL_DEFAULT,
										&pTexture0,
										NULL))) {
											return E_FAIL;
									}
							}
					}
			}
	}

	// テクスチャからレンダリングターゲットにするサーフェースの取得
	if( FAILED(pTexture0->GetSurfaceLevel(0, &pTexSurface0))){
		return E_FAIL;
	}

	// テクスチャへのレンダリングに使う深度バッファーの作成
	if (FAILED(GetDevice()->CreateDepthStencilSurface(
						width, height,		
						D3DFMT_D24X8,
						D3DMULTISAMPLE_NONE,
						0,
						TRUE,
						&pTexZ0,
						NULL))){
		DXTRACE_ERR("<レンダーターゲットのための深度バッファ作成>",hr);
		return E_FAIL;
	}
	return S_OK;
}
HRESULT RenderTarget::ResetSurface(){
	if(width&&height){
		if(FAILED(InitSurface(width,height))){
			return E_FAIL;
		}
	}
	return S_OK;
}
void RenderTarget::ReleaseSurface(){
	SAFE_RELEASE(pTexture0);
	SAFE_RELEASE(pTexSurface0);
	SAFE_RELEASE(pTexZ0);
}
HRESULT RenderTarget::BeginToggleBackBuffer(){

	//========================================
	// 準備：バックバッファーの退避
	//========================================

	// ビューポート情報の保持
	GetDevice()->GetViewport(&vpOrig);

	// バックバッファーへのインターフェースの保持
	GetDevice()->GetRenderTarget(0, &pBackbuffer);

	// 深度バッファーのインターフェースの保持
    GetDevice()->GetDepthStencilSurface( &pBackZ);

	//========================================
	//パス１：シーンの深度だけを描画
	//========================================

	// テクスチャをレンダリングターゲットに設定
	GetDevice()->SetRenderTarget(0, pTexSurface0);

	// テクスチャ用の深度バッファを設定
	GetDevice()->SetDepthStencilSurface(pTexZ0);

	// ビューポートをテクスチャ画像のサイズに設定
	D3DVIEWPORT9 viewport2 = { 0,0,width,height, 0.0f, 1.0f};
	GetDevice()->SetViewport(&viewport2);

	if(FAILED(GetDevice()->Clear(0,NULL, 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f),
		1.0f, 0))) {
			return E_FAIL;
	}
	return S_OK;

}
HRESULT RenderTarget::EndToggleBackBuffer(){
	

	//==========================================
	//ターゲットを戻す
	//==========================================

	// 保存しておいたバックバッファをレンダリングターゲットに設定
	GetDevice()->SetRenderTarget(0, pBackbuffer);

	// 保存しておいたバックバッファの深度バッファを設定
	GetDevice()->SetDepthStencilSurface(pBackZ);

	// 保存しておいたビューポートを元に戻す
	GetDevice()->SetViewport(&vpOrig);

	SAFE_RELEASE(pBackbuffer);
	SAFE_RELEASE(pBackZ);

	return S_OK;

}

void SetOffset(){
}

LPDIRECT3DTEXTURE9 RenderTarget::GetTexture(){
	return pTexture0;
}
int RenderTarget::GetTargetWidth(){
	return width;
}
int RenderTarget::GetTargetHeight(){
	return height;
}