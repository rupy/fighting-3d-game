#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include <d3dx9.h>
#include "ResourceBase.h"

class RenderTarget:public ResourceBase{
	
	// バックバッファーの退避用
	D3DVIEWPORT9			vpOrig;
	LPDIRECT3DSURFACE9      pBackbuffer;	// バックバッファー
    LPDIRECT3DSURFACE9		pBackZ;		// バックバッファー用の深度バッファー

	//レンダリングターゲット（オリジナルサイズ）
	int width;
	int height;
	LPDIRECT3DTEXTURE9      pTexture0;		// テクスチャ
	LPDIRECT3DSURFACE9      pTexSurface0;	// テクスチャから取得するサーフェース
    LPDIRECT3DSURFACE9		pTexZ0;		// テクスチャ用の深度バッファー

public:
	RenderTarget(LPDIRECT3DDEVICE9 device);
	~RenderTarget();


	HRESULT InitSurface(int width,int height);
	void ReleaseSurface();
	HRESULT ResetSurface();
	HRESULT BeginToggleBackBuffer();
	HRESULT EndToggleBackBuffer();

	void SetOffset();

	LPDIRECT3DTEXTURE9 GetTexture();
	int GetTargetWidth();
	int GetTargetHeight();
};


#endif	/*__RENDER_TARGET_H__*/