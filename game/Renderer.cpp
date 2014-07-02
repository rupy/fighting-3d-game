//=============================================================
// Renderer.cpp
// レンダラークラスの実装
//=============================================================

#include "Renderer.h"
#include "Common.h"
//=============================================================
// 実装
//=============================================================
//-------------------------------------------------------------
// デフォルトコンストラクタ
//-------------------------------------------------------------
Renderer::Renderer()
 : pD3D(NULL), pDevice(NULL),pSprite(NULL)
{
}

//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
Renderer::~Renderer()
{
	Finalize();
}

//-------------------------------------------------------------
// レンダラーの初期化
// 引数
//		hWnd         : ウィンドウハンドル
//		isFullScreen : フルスクリーンの場合TRUE
//		clientWidth  : クライアント領域の幅
//		clientHeight : クライアント領域の高さ
// 戻り値
//		成功したらS_OK 
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{

    D3DDISPLAYMODE d3ddm;
    
	// Direct3D9オブジェクトの作成
	if((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
        return E_FAIL;	// 取得失敗
    }
	
	// 現在のディスプレイモードを取得
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	//デバイスの能力を調べる
	//D3DCAPS9 caps;
	//if(FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps))){
	//	return E_FAIL;
	//}

	// デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	D3DPP.BackBufferCount			= 1;
	if(isFullScreen) { // フルスクリーンの場合
		D3DPP.Windowed				= FALSE;			// フルスクリーン表示の指定
		D3DPP.BackBufferWidth		= clientWidth;		// フルスクリーン時の横幅
		D3DPP.BackBufferHeight		= clientHeight;		// フルスクリーン時の縦幅
	}
	else {
		D3DPP.Windowed				= TRUE;				// ウインドウ内表示の指定
	}
	D3DPP.BackBufferFormat		= d3ddm.Format;			// カラーモードの指定
	D3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD; //最も効率的なスワップチェーンの表現を選択
	D3DPP.EnableAutoDepthStencil	= TRUE;
	D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;
	D3DPP.hDeviceWindow				= hWnd;
	
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&D3DPP, &pDevice))) {
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&D3DPP, &pDevice))) {
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&D3DPP, &pDevice))) {
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	//スプライトの作成
	if(FAILED(D3DXCreateSprite(pDevice,&pSprite))){
		return(E_FAIL);
	}

	return S_OK;
}

//---------------------------------------------------------
// デバイスの取得
// 戻り値
//		成功したらIDirect3DDevice9インターフェイスへのポインタ
//		失敗したら0
//---------------------------------------------------------
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return pDevice;
}

D3DPRESENT_PARAMETERS* Renderer::GetPParam()
{
	return (&D3DPP);
}

LPD3DXSPRITE Renderer::GetSprite()
{
	return (pSprite);
}


//-------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------
void Renderer::Finalize()
{
	// デバイスオブジェクトの解放
	SAFE_RELEASE(pDevice);

	// DirectXGraphicsの解放
	SAFE_RELEASE(pD3D);

	//スプライトの解放
	SAFE_RELEASE(pSprite);

}