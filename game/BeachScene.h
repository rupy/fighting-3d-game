#ifndef __BEACH_SCENE_H__
#define __BEACH_SCENE_H__

#include "Scenebase.h"
#include "StartScene.h"
#include "Mesh.h"
#include "TerrianObject.h"
#include "AnimMesh.h"
#include "AnimObject.h"
#include "Human.h"
#include "MeshPlayer.h"
#include "MeasureLine.h"
#include "Rain.h"
#include "Fire.h"
#include "Sphere.h"
#include "FlipAnimObject.h"
#include "StatusMenu2.h"
#include "AnimCPU.h"

#include "MySpriteObject.h"
#include "WaterField.h"
#include "StatusMeter.h"
#include "BillboardObject.h"
#include "Magic.h"
#include "CameraOperator.h"
#include "VectorTest.h"
#include "MsgBox.h"
#include "Snow.h"
#include "Rain.h"


class BeachScene :public SceneBase{


	enum SceneMode{
		FIELD,
		TALK,
		STATUS,
		GO_TITLE,
	};


public:
	
	//コンストラクタ
	BeachScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm);
	//デストラクタ
	~BeachScene();

	//初期化
	//ゲームの実行
	void Draw();
private:

	//=============================================================
	// 初期化
	//=============================================================
	HRESULT Initialize();

	HRESULT SetupShader();
	void HitDefine();
	HRESULT Prepare();
	


	//ライトやカメラに関すること
	//=============================================================
	//HRESULT SetupCamera();
	void LightMove();

	//マップに関すること
	//=============================================================
	TerrianObject mapObj;//マップ
	WaterField	sea;//海
	MeshObject sky;//空
	HRESULT SetupMap();
	void MapMove();
	void MapGravity();
	void MapWall();

	//プレイヤーに関すること
	//=============================================================
	AnimObject hitoObj;
	Human hitoPlayer;
	MeshObject carObj;
	MeshPlayer carPlayer;
	CameraOperator camOp;
	bool command;
	HRESULT SetupPlayer();//プレーヤーキャラを初期化
	void PlayerMove(KeyMessage* keyMessage);
	AttachObject sword;//当たり判定の表示

	//敵キャラに関すること
	//=============================================================
	AnimObject hitoObj2;
	AnimCPU	hitoCPU;
	HRESULT SetupEnemy();//敵キャラの初期化
	void EnemyMove();

	//エフェクトに関すること
	//=============================================================
	Fire rainObject;
	BillboardObject bill;
	Magic* heal;
	Snow snow;
	Rain rain;
	HRESULT SetupEffect();
	void EffectMove();

	//動作テストに関すること
	//=============================================================
	MySpriteObject shadowtest;//シャドウマップの表示
	MeasureLine lines;//グリッドの表示
	VectorTest arrowObj;//進行方向の表示
	HRESULT SetupTest();
	void TestMove();

	//表示部に関すること
	//=============================================================
	StatusMenu status_menu;
	StatusMeter HPMeter;
	StatusMeter MPMeter;
	MsgBox		msg;
	HRESULT SetupDisplay();
	void DisplayMove();

	//サウンドに関すること
	//=============================================================
	SoundData SE[NUM_OF_SOUNDTYPE];// 効果音
	HRESULT SetupSound();

	void CheckHit();
	void Animation();
	void AllMove();


	//=============================================================
	// ゲームの実行
	//=============================================================
	SceneBase* Move(KeyMessage* keyMessage);
	void FieldMode(KeyMessage* keyMessage);
	void StatusMode(KeyMessage* keyMessage);
	void TalkMode(KeyMessage* keyMessage);

	//描画
	void DrawShadow();
	void DrawObject();

};


#endif	/*__BEACH_SCENE_H__*/