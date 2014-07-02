#ifndef __EFFECT_H__
#define __EFFECT_H__

#include <d3dx9.h>
#include <tchar.h>
#include <map>
#include <string>
#include <vector>
#include "common.h"

using namespace std;

struct EffectInfo{
	string	fileName;
	string	techName;
	vector<string>	valueName;
};

class Effect{
public:
	LPD3DXEFFECT	pFX;
	LPDIRECT3DDEVICE9 pDevice;

	EffectInfo info;

	// テクニックへのハンドル
	D3DXHANDLE              hTech;

	// シェーダプログラムのグローバル変数へのハンドル
	map<string,D3DXHANDLE>	hValue;

	// バックバッファーの退避用
	D3DVIEWPORT9 viewport1;
	LPDIRECT3DSURFACE9      pBackbuffer;	// バックバッファー
    LPDIRECT3DSURFACE9		pBackZ;		// バックバッファー用の深度バッファー

	//レンダリングターゲット（オリジナルサイズ）
	LPDIRECT3DTEXTURE9      pTexture0;		// テクスチャ
	LPDIRECT3DSURFACE9      pTexSurface0;	// テクスチャから取得するサーフェース
    LPDIRECT3DSURFACE9		pTexZ0;		// テクスチャ用の深度バッファー


	Effect();
	~Effect();
	HRESULT InitEffect(LPDIRECT3DDEVICE9 device,const EffectInfo* info);
	LPD3DXEFFECT GetEffect();
	HRESULT Reset();

	HRESULT SetMatrix(const D3DXMATRIX* mat ,char* handle_name);
	HRESULT SetVector(const D3DXVECTOR4* vec ,char* handle_name);
	HRESULT SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name);
	HRESULT SetTechnique();

	HRESULT InitSurface();
	HRESULT BeginToggleBackBuffer();
	HRESULT EndToggleBackBuffer();

private:
	HRESULT EffectCompile();
	HRESULT HandleSetting();

};


#endif	/*__EFFECT_H__*/