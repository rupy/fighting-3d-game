#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__

#include <windows.h>
#include "Light.h"
#include "Input.h"
#include "Sound.h"
#include "Effect.h"
#include <vector>
#include "SaveManager.h"
#include "RenderTarget.h"
#include "Camera.h"

class SceneBase{

public:
	bool					canClear;	//画面をクリアしてもよいか（ムービー再生中などはクリアするとちかちかなる）
	Light*					light;		//ライトを管理する構造体
	vector<Effect*>			eff;		//エフェクトデバイス（シェーダーファイルの数だけつくる）
	vector<RenderTarget*>	target;		//切り替えレンダリングターゲット
	vector<Camera*>			camera;		//カメラ（視点の数だけ用意する）

	D3DPRESENT_PARAMETERS* pD3DPPP;		//Direct3Dパラメーター
protected:
	int sceneMode;						//シーンモード

	//=============================
	//デバイスなど
	//=============================
	LPD3DXSPRITE			pSprite;	//スプライトデバイス
	LPDIRECT3DDEVICE9		pDevice;	//DirectXデバイス
	Sound*					pSound;		//サウンドデバイス
	SaveManager*			pSM;		//セーブデータ

public:
	SceneBase(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm);
	virtual ~SceneBase();

	//初期化処理
	virtual HRESULT Initialize()=0;

	//ゲームの実行
	virtual SceneBase* Move(KeyMessage* keyMessage)=0;
	//描画
	virtual void Draw()=0;
	//2D描画のためにクリアする（ビルボード含む）
	void ClearFor2DDraw();
	//2Dのためにレンダーステートを設定する（ビルボード含まない）
	void SetRenderStateFor2D();
};


#endif	/*__SCENEBASE_H__*/