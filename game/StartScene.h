#ifndef __STARTSCENE_H__
#define __STARTSCENE_H__

#include "Scenebase.h"
#include "BeachScene.h" 
#include "StringObject2.h"
#include "Movie.h"
#include "MySpriteObject.h"
#include "Error.h"

class BeachScene;

#define CURSOR_MOVE	32


const int TOP = 50;									//上側の空白
const int LEFT = 50;								//左側の空白
const int col_space = 32;							//行幅
const int col_num = (WINDOW_HEIGHT - TOP)/col_space;		//行数
const int raw_space = 250;							//列数
class StartScene :public SceneBase{

	enum SceneMode{
		START_MOVIE,
		PLAY_LOGO_MOVIE,
		MOVIE_END,
		TITLE,
		SCENE_SELECT,
		CHANGE_NEXT,
		GAME_CONFIG,
	};

	//=============================
	//ムービー画面
	//=============================
	Movie *mv;

	//=============================
	//タイトル画面
	//=============================
	enum SelectMode{
		GAME_START,
		CONTINUE,
		CONFIG,
		GAME_END,
	};

	int curPos;//カーソール位置
	int curPos2;//カーソール位置

	//タイトル背景
	LPDIRECT3DTEXTURE9 titlePic;
	MySpriteObject* titleSprite;
	//ボタン用(GameStart)
	LPDIRECT3DTEXTURE9 startBtnPic;
	MySpriteObject* startBtn;
	//ボタン用(Continue)
	LPDIRECT3DTEXTURE9 continueBtnPic;
	MySpriteObject* continueBtn;
	//ボタン用(Config)
	LPDIRECT3DTEXTURE9 configBtnPic;
	MySpriteObject* configBtn;
	//ボタン用(GameEnd)
	LPDIRECT3DTEXTURE9 endBtnPic;
	MySpriteObject* endBtn;
	//カーソール用
	LPDIRECT3DTEXTURE9 selectPic;
	MySpriteObject* selectCur;


	//=============================
	//コンフィグ画面
	//=============================

	//背景
	LPDIRECT3DTEXTURE9 confPic;
	MySpriteObject* confSprite;
	
	//=============================
	//フェードアウト
	//=============================
	LPDIRECT3DTEXTURE9 blackPic;
	MySpriteObject* blackOut;

	//=============================
	//セレクト画面
	//=============================
	LPDIRECT3DTEXTURE9 sceneSelectPic;
	MySpriteObject* sceneSelectSprite;
	vector<StringObject> files;
	vector<string> filenames;
	MySpriteObject* selectCur2;


public:
	StartScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm);
	~StartScene();

	HRESULT Initialize();
	SceneBase* Move(KeyMessage* keyMessage);
	void Draw();
private:
	HRESULT SetupTextures();
	HRESULT SetupModels();
	HRESULT SetupObjects();
	HRESULT SetupMovies();
	HRESULT Prepare();
};


#endif	/*__STARTSCENE_H__*/