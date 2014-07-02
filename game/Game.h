#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include "StartScene.h"
#include "BeachScene.h"
#include "Input.h"
#include "Sound.h"
#include "SaveManager.h"

enum SceneNum{
	SCENE0,
	SCENE1,
	SCENE2,
	SCENE3,
	SCENE4,
	SCENE5,
	INVALID
};

class GameBase{

public:
	SceneBase*				scene;
	SaveManager				SM;

	LPDIRECT3DDEVICE9		pDevice;
	D3DPRESENT_PARAMETERS*	pD3DPP;
	LPD3DXSPRITE			pSprite;
	Input*					pInput;
	Sound*					pSound;


public:
	GameBase(
		LPDIRECT3DDEVICE9		device,
		D3DPRESENT_PARAMETERS*	param,
		LPD3DXSPRITE			sprite,
		Input*					input,
		Sound*					sound
		);

	~GameBase();

	//ゲームの実行
	int Run();
	HRESULT Draw();
	HRESULT Display();


	//デバイスロストなど
	void RecetRenderState();
	HRESULT DeviceRecet();
};


#endif	/*__GAME_H__*/