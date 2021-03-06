//=============================================================
// Renderer.h
// レンダラークラスの定義
//=============================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3dx9.h>

//=============================================================
// Renderer
// レンダラークラス
//=============================================================
class Renderer
{

private:
	LPDIRECT3D9				pD3D;			// IDirect3D9インターフェイスへのポインタ
	LPDIRECT3DDEVICE9		pDevice;		// IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS	D3DPP;			// デバイスのプレゼンテーションパラメータ
	LPD3DXSPRITE			pSprite;		// Sprite

public:
	//---------------------------------------------------------
	// デフォルトコンストラクタ
	//---------------------------------------------------------
	Renderer();

	//---------------------------------------------------------
	// デストラクタ
	//---------------------------------------------------------
	~Renderer();

public:
	//---------------------------------------------------------
	// レンダラーの初期化
	// 引数
	//		hWnd         : ウィンドウハンドル
	//		isFullScreen : フルスクリーンの場合TRUE
	//		clientWidth  : クライアント領域の幅
	//		clientHeight : クライアント領域の高さ
	// 戻り値
	//		成功したらS_OK 
	//---------------------------------------------------------
	HRESULT Initialize(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight);

	//---------------------------------------------------------
	// デバイスの取得
	// 戻り値
	//		成功したらIDirect3DDevice9インターフェイスへのポインタ
	//		失敗したら0
	//---------------------------------------------------------
	LPDIRECT3DDEVICE9 GetDevice();
	D3DPRESENT_PARAMETERS* GetPParam();
	LPD3DXSPRITE GetSprite();

	//---------------------------------------------------------
	// 終了処理
	//---------------------------------------------------------
	void Finalize();

};

#endif //_RENDERER_H_