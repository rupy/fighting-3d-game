#include "BeachScene.h"
#include "HitTest.h"


BeachScene::BeachScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm)
:SceneBase(device,sprite,sound,sm),
//マップの初期化
mapObj(eff),
sea(eff),
sky(eff),
//プレイヤーキャラの初期化
hitoObj(eff,NULL,1,D3DXVECTOR3(10.0f,100.0f,-5.0f)),
hitoPlayer(camera[0],&hitoObj),
carObj(eff,NULL,D3DXVECTOR3(0.0f,100.0f,-5.0f)),
carPlayer(camera[0],&carObj),
camOp(camera[0],&hitoObj),
sword(eff,NULL),command(true),
//敵キャラの初期化
hitoObj2(eff,NULL,1,D3DXVECTOR3(0.0f,100.0f,5.0f)),
hitoCPU(&hitoObj2),
//エフェクトの初期化
bill(eff,NULL,D3DXVECTOR3(0,5.0f,0)),
rainObject(50,true),
heal(NULL),
snow(10.0f,100),
rain(10.0f,100),
//動作テストの初期化
arrowObj(eff),
shadowtest(300,300),
lines(),
//表示部の初期化
status_menu(&sm->state.party),
HPMeter(D3DXVECTOR3(20.0f,120.0f,0)),
MPMeter(D3DXVECTOR3(20.0f,100.0f,0))
{
}
BeachScene::~BeachScene(){
	
	pSound->StopASound(&SE[0]);
	SAFE_DELETE(heal);
}
HRESULT BeachScene::Initialize(){

	//ライトの初期化
	light = new Light(eff,D3DXVECTOR3(0.0f,0.0f,0.0f));
//	camera.push_back(new Camera(eff,
//						D3DXVECTOR3( 15.0f, 15.0f, -15.0f), 
//						D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 
//						D3DXVECTOR3( 0.0f, 1.0f, 0.0f)));

	//シェーダーの初期化
	if(FAILED(SetupShader())){
		return E_FAIL;
	}
	//マップの初期化
	if(FAILED(SetupMap())){
		return E_FAIL;
	}
	//プレイヤーキャラを初期化
	if(FAILED(SetupPlayer())){
		return E_FAIL;
	}
	//敵キャラを初期化
	if(FAILED(SetupEnemy())){
		return E_FAIL;
	}
	//エフェクトを初期化
	if(FAILED(SetupEffect())){
		return E_FAIL;
	}
	//動作テストを初期化
	if(FAILED(SetupTest())){
		return E_FAIL;
	}
	//動作テストを初期化
	if(FAILED(SetupDisplay())){
		return E_FAIL;
	}
	//動作テストを初期化
	if(FAILED(SetupSound())){
		return E_FAIL;
	}

	//当たり判定の定義
	HitDefine();

	//その他の準備
	if(FAILED(Prepare())){
		return E_FAIL;
	}
	return(S_OK);
}
//=============================================================
// マップを初期化
//=============================================================
HRESULT BeachScene::SetupMap(){

	//マップ
	if(pSM->state.file.empty()){
		pSM->state.file = "models/map/moeghi.x";
	}
	mapObj.LoadMeshFromXFile(pSM->state.file.c_str(),"models/map/");//mapObj.Pos.z -= 5.0f;
	//mapObj.Roll += D3DX_PI/10.0f;
	sword.LoadMeshFromXFile("models/sword.x");

	sky.LoadMeshFromXFile("models/sky.x");
	sky.use_shadow = false;
	sky.Scale *= 3.0f;
	return(S_OK);
}
//=============================================================
// プレイヤーキャラを初期化
//=============================================================
HRESULT BeachScene::SetupPlayer(){

	//人間
	hitoObj.LoadMeshFromXFile("models/hito6.x");

	//車
	carObj.LoadMeshFromXFile("models/car.x");

	// 落下
	mapObj.Landing(&hitoObj,0.9f);
	mapObj.Landing(&carObj,1.5f);

	return(S_OK);
}
//=============================================================
// 敵キャラを初期化
//=============================================================
HRESULT BeachScene::SetupEnemy(){

	hitoObj2.SetMesh(hitoObj.GetMesh()->Clone());
	//目標設定
	hitoCPU.SetTargetObject(&hitoObj);

	//落下
		mapObj.Landing(&hitoObj2,0.9f);

	return(S_OK);
}
//=============================================================
// エフェクトを初期化
//=============================================================
HRESULT BeachScene::SetupEffect(){

	bill.SetupTexture("picture/star.png");
	bill.parentObj = &hitoObj;


	return(S_OK);
}
//=============================================================
// 動作テストを初期化
//=============================================================
HRESULT BeachScene::SetupTest(){

	//ベクトルのテスト
	arrowObj.parentObj = &hitoObj;

	return(S_OK);
}
//=============================================================
// 表示部を初期化
//=============================================================
HRESULT BeachScene::SetupDisplay(){

	return(S_OK);
}
//=============================================================
//エフェクトの読み込み
//=============================================================
HRESULT BeachScene::SetupShader(){

	eff.push_back(new Effect);
	EffectInfo info;
	info.fileName = "shadow.fx";
	info.techName = "BasicTech";
	info.valueName.push_back("g_world");
	info.valueName.push_back("g_world_inv");
	info.valueName.push_back("g_view");
	info.valueName.push_back("g_proj");
	info.valueName.push_back("g_color0");
	info.valueName.push_back("g_color1");
	info.valueName.push_back("g_light_dir");
	info.valueName.push_back("g_texture");
	info.valueName.push_back("g_shadowbuf");
	info.valueName.push_back("g_offset");
	info.valueName.push_back("g_vlp");
	info.valueName.push_back("g_use_shadow");
	info.valueName.push_back("g_fog");
	//エフェクトの読み込み
	if(FAILED(eff.back()->InitEffect(&info))){
		return E_FAIL;
	}
	eff.push_back(new Effect);
	EffectInfo info_s;
	info_s.fileName = "shadowmap.fx";
	info_s.techName = "BasicTech";
	info_s.valueName.push_back("g_world");
	info_s.valueName.push_back("g_vlp");
	//エフェクトの読み込み
	if(FAILED(eff.back()->InitEffect(&info_s))){
		return E_FAIL;
	}
	target.push_back(new RenderTarget(pDevice));

	if(FAILED(target.back()->InitSurface(2560,1880))){
		return E_FAIL;
	}
	

	//フォグの設定
	D3DXVECTOR4 fog = D3DXVECTOR4(1000.0f/(1000.0f-1.0f),-1.0f/(1000.0f-1.0f),0,0);// (Far/(Far-Near), -1/(Far-Near))
	if(FAILED(eff[0]->SetVector(&fog,"g_fog"))){
		return E_FAIL;
	}

	return S_OK;
}
//=============================================================
//サウンドの読み込み
//=============================================================
HRESULT BeachScene::SetupSound(){
	
	if(FAILED(pSound->LoadFromWaveFile("sound/wave.wav",&SE[0],DSBCAPS_CTRLVOLUME|DSBCAPS_GLOBALFOCUS,GUID_NULL,4))){
		MessageBox( NULL, "サウンドの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	pSound->PlayASound(&SE[0],0,DSBPLAY_LOOPING,TRUE);
	return S_OK;
}
//=============================================================
//当たり判定の定義
//=============================================================
void BeachScene::HitDefine(){
		//攻撃判定
	Hit HitAttack[]={
		{"LeftHand",0.8f},
		{"RightHand",1.0f},
		{"LeftFoot",0.8f},
		{"RightFoot",0.8f},
		{"Head",0.8f}
	};
	hitoObj.GetMesh()->SetHitAttack(HitAttack,sizeof(HitAttack)/sizeof(Hit));

	//防御判定
	Hit HitDefense[]={
		{"Parent",0.3f},
		{"Waist",0.3f},
		{"Stomach",0.3f},
		{"Breast",0.3f},
		{"LeftShoulder",0.3f},
		{"LeftUpperArm",0.3f},
		{"LeftLowerArm",0.3f},
		{"LeftHand",0.3f},
		{"RightShoulder",0.3f},
		{"RightUpperArm",0.3f},
		{"RightLowerArm",0.3f},
		{"RightHand",0.3f},
		{"Neck_Parent_",0.3f},
		{"Neck",0.3f},
		{"Head_Parent_",0.3f},
		{"Head",0.3f},
		{"LeftUpperKnee_Parent_",0.3f},
		{"LeftUpperKnee",0.3f},
		{"LeftLowerKnee",0.3f},
		{"LeftFoot_Parent_",0.3f},
		{"LeftFoot",0.3f},
		{"RightUpperKnee_Parent_",0.3f},
		{"RightUpperKnee",0.3f},
		{"RightLowetKnee_Parent_",0.3f},
		{"RightLowetKnee",0.3f},
		{"RightFoot_Parent_",0.3f},
		{"RightFoot",0.3f}
	};
	hitoObj.GetMesh()->SetHitDefense(HitDefense,sizeof(HitDefense)/sizeof(Hit));
	hitoObj.GetMesh()->CalcBoundingSphere();
}
//=============================================================
//準備
//=============================================================
HRESULT BeachScene::Prepare(){

	sceneMode = FIELD;
	return S_OK;
}
//=============================================================
//移動
//=============================================================
SceneBase* BeachScene::Move(KeyMessage* keyMessage){

	switch(sceneMode){
		case FIELD:
			FieldMode(keyMessage);
			break;
		case STATUS:
			StatusMode(keyMessage);
			break;
		case TALK:
			TalkMode(keyMessage);
			break;
		case GO_TITLE:
			return new StartScene(pDevice,pSprite,pSound,pSM);
			break;
		default:
			break;
	}

	return 0;
}

//=============================================================
//フィールド
//=============================================================
void BeachScene::FieldMode(KeyMessage* keyMessage){

	//ライトの移動
	LightMove();
	//プレイヤーの移動
	PlayerMove(keyMessage);
	//敵の移動
	EnemyMove();
	//エフェクトの動作
	EffectMove();
	//マップの動作
	MapMove();
	//壁
	MapWall();
	//重力
	MapGravity();
	//最終的な当たり判定
	CheckHit();
	//すべての物体の移動
	AllMove();
	//テストの動作
	TestMove();
	//表示部の動作
	DisplayMove();
	//アニメーション
	Animation();
	
	if(keyMessage->cmd & NEW_SELECT){
		msg.SelectMsg("start");
		sceneMode = TALK;
		command = false;
	}
}
//=============================================================
//ステータス
//=============================================================
void BeachScene::StatusMode(KeyMessage* keyMessage){

	if(keyMessage->cmd & NEW_START){
		sceneMode = FIELD;
	}
}

//=============================================================
//会話
//=============================================================
void BeachScene::TalkMode(KeyMessage* keyMessage){
	
	EffectMove();
	Animation();
	msg.Move(keyMessage);
	if(keyMessage->cmd & NEW_A){
		int next;
		if(!msg.NextMsg(&next)){
			switch(next){
				case -1:
					sceneMode = GO_TITLE;
					break;
				case 0:
					sceneMode = FIELD;
					break;
				case 1:
					sceneMode = FIELD;
					break;
			}
		}
	}
}

//=============================================================
//プレイヤーの動作
//=============================================================
void BeachScene::PlayerMove(KeyMessage* keyMessage){
	//移動
	hitoPlayer.Move(keyMessage);
//	carPlayer.Move(keyMessage);
	camOp.Move(keyMessage);
	if(keyMessage->cmd & NEW_Y){
		if(!heal&&pSM->state.party.member1.UseMP(3)){
			pSM->state.party.member1.AllHealHP();
			heal = new Magic(eff,100);
			heal->SetParent(&hitoObj);
		}
	}/*
	if(keyMessage->cmd & NEW_START){
		status_menu.SetData();
		sceneMode = STATUS;
	}*/
}
//=============================================================
//ライトの動作
//=============================================================
void BeachScene::LightMove(){

	//光の移動
	light->Pos = hitoObj.Pos;
	DWORD time = timeGetTime();
	light->Pos.x += -20.0f;
	light->Pos.y += 15.0f; 
	light->Pos.z += -20.0f;
	light->lookatPt = hitoObj.Pos;
}
//=============================================================
//敵の動作
//=============================================================
void BeachScene::EnemyMove(){
	//移動
	hitoCPU.Move();

}
//=============================================================
//エフェクトの動作
//=============================================================
void BeachScene::EffectMove(){
	//付随
	rainObject.Pos = hitoObj2.Pos;
	rainObject.Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);
	bill.LookAtMe(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);

	rain.Move();
	snow.Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);

	if(heal){
		if(heal->Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt)){
			SAFE_DELETE(heal);
		}
	}
}
//=============================================================
//テストの動作
//=============================================================
void BeachScene::TestMove(){

	//arrowObj.Pos.z=-1.0f;
	sword.SetParentMatrix(hitoObj.GetAttachMatrix("RightHand"));
	shadowtest.pTex = target.back()->GetTexture();
	VectorTest::SetDir(hitoObj.Velo);
	lines.Pos = hitoObj.Pos;
}
//=============================================================
//マップの動作
//=============================================================
void BeachScene::MapMove(){
	sea.Move();
	sky.Pos = hitoObj.Pos;
	sky.Pos.y =-100.0f;
	//mapObj.Roll += D3DX_PI/10000.0f;
}
//=============================================================
//マップの重力
//=============================================================
void BeachScene::MapGravity(){
	hitoPlayer.nowJump = !mapObj.LandHit(&hitoObj,!hitoPlayer.nowJump,0.9f);
	mapObj.LandHit(&hitoObj2,false,0.9f);
	//mapObj.Gravity(&carObj,false,1.3f);
	//mapObj.Gravity(&hitoObj2.Pos,true,0.9f);
	//mapObj.Gravity(&hitoPlayer.cam->eyePt,false,0.9f);
}
//=============================================================
//マップなどの壁判定
//=============================================================
void BeachScene::MapWall(){
	mapObj.WallHit(&hitoObj,10.0f,0.0f);
	mapObj.WallHit(&hitoObj2,7.0f,0.5f);
}
//=============================================================
//当たり判定によるダメージなど
//=============================================================
void BeachScene::CheckHit(){
	
	//剣の攻撃
	if(hitoObj.GetAnimIndex() == 8 && HitTest(&hitoObj,1,&hitoObj2) && hitoObj2.GetDamage()){
		hitoCPU.HP--;
		hitoObj2.NoDamage();
		hitoObj2.SetAnimIndex(7);
//		hitoObj2.AnimStart();
		hitoObj2.SetAnimSpeed(1);
		hitoObj2.NoAnimChange();
	}

	//敵
	//パンチ攻撃
	if(hitoObj2.GetAnimIndex() == 3 && HitTest(&hitoObj2,0,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(1);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	//キック攻撃
	if(hitoObj2.GetAnimIndex() == 5 && HitTest(&hitoObj2,2,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(1);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	//頭突き攻撃
	if(hitoObj2.GetAnimIndex() == 6 && HitTest(&hitoObj2,4,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(2);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	if(hitoCPU.HP == 0){
		hitoObj2.SetAnimIndex(9);
		hitoObj2.AnimStop();
	}
	if(pSM->state.party.member1.GetHP() == 0){
		hitoObj.SetAnimIndex(9);
		hitoObj.AnimStop();
	}



}
void BeachScene::AllMove(){
	if(command){
		hitoObj.Move();
		hitoObj2.Move();
	}else{
		//コマンド制限は一度きりですぐ元に戻す
		//画面切り替えの時に同時にジャンプしたりするのを防ぐ目的
		command = true;
	}
}
//=============================================================
//表示部の動作
//=============================================================
void BeachScene::DisplayMove(){

		HPMeter.SetMeter(pSM->state.party.member1.GetHP(),
		pSM->state.party.member1.GetMaxHP());
	MPMeter.SetMeter(pSM->state.party.member1.GetMP(),
		pSM->state.party.member1.GetMaxMP());
}
//=============================================================
//表示部の動作
//=============================================================
void BeachScene::Animation(){

	hitoObj.Animation();
	hitoObj2.Animation();
}
//=============================================================
//描画
//=============================================================
void BeachScene::Draw(){
	//cam->CameraSetting();

	DrawShadow();

	DrawObject();
}

//=============================================================
//影の描画
//=============================================================
void BeachScene::DrawShadow(){


	light->LightSetting();
	//レンダリングターゲットをテクスチャに切り替える

	target.back()->BeginToggleBackBuffer();

	mapObj.Shadow();
	hitoObj.Shadow();
	//carObj.Shadow();
	hitoObj2.Shadow();
	//arrowObj.Shadow();
	sword.Shadow();
	target.back()->EndToggleBackBuffer();
	// シャドウマップの設定
	if(FAILED(eff[0]->SetTexture(target.back()->GetTexture(),"g_shadowbuf"))){
		return ;
	}
	// シャドウマップへアクセスするときのオフセット設定
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f/target.back()->GetTargetWidth();
	tmpOffset.y = 0.5f/target.back()->GetTargetHeight();
	if(FAILED(eff[0]->SetVector(&tmpOffset,"g_offset"))){
		return;
	}
}
//=============================================================
//オブジェクトの描画
//=============================================================
void BeachScene::DrawObject(){

	switch(sceneMode){
		case FIELD:
		case TALK:
			mapObj.Appear();
			hitoObj.Appear();
			//carObj.Appear();
			//arrowObj.Appear();
			sword.Appear();
			hitoObj2.Appear();
			//lines.Appear();
			break;
	}

	//==================================
	//アルファ値を持つオブジェクトを描画
	//==================================
	switch(sceneMode){
		case FIELD:
		case TALK:
			rain.Appear();
			sea.Appear();
			rainObject.Appear();
			
			//==================================
			//ビルボードの描画
			//==================================
			SetRenderStateFor2D();
			//bill.Appear();
			if(heal){
				heal->Appear();
			}
			snow.Appear();
			break;
	}
	//==================================
	//表示部などの2D部分の描画
	//==================================
	ClearFor2DDraw();
	switch(sceneMode){
		case FIELD:
			//shadowtest.Appear();
			HPMeter.Appear();
			MPMeter.Appear();
			break;
		case STATUS:
			status_menu.Appear();
			break;
		case TALK:
			msg.Appear();
			break;
	}

}