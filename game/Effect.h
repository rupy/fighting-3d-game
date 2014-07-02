#ifndef __EFFECT_H__
#define __EFFECT_H__

#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>
#include "Common.h"
#include "ResourceBase.h"

using namespace std;

struct EffectInfo{
	string	fileName;
	string	techName;
	vector<string>	valueName;
};

class Effect:public ResourceBase{
public:
	LPD3DXEFFECT	pFX;

	EffectInfo info;

	// テクニックへのハンドル
	D3DXHANDLE              hTech;

	// シェーダプログラムのグローバル変数へのハンドル
	map<string,D3DXHANDLE>	hValue;

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


	Effect();
	~Effect();
	HRESULT InitEffect(const EffectInfo* info);
	LPD3DXEFFECT GetEffect();
	HRESULT Reset();

	HRESULT SetMatrix(const D3DXMATRIX* mat ,char* handle_name);
	HRESULT SetVector(const D3DXVECTOR4* vec ,char* handle_name);
	HRESULT SetBool(const bool* value ,char* handle_name);
	HRESULT SetFloat(const float* value ,char* handle_name);
	HRESULT SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name);
	HRESULT SetTechnique();


private:
	HRESULT EffectCompile();
	HRESULT HandleSetting();

};


#endif	/*__EFFECT_H__*/