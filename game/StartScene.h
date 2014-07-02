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


const int TOP = 50;									//�㑤�̋�
const int LEFT = 50;								//�����̋�
const int col_space = 32;							//�s��
const int col_num = (WINDOW_HEIGHT - TOP)/col_space;		//�s��
const int raw_space = 250;							//��
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
	//���[�r�[���
	//=============================
	Movie *mv;

	//=============================
	//�^�C�g�����
	//=============================
	enum SelectMode{
		GAME_START,
		CONTINUE,
		CONFIG,
		GAME_END,
	};

	int curPos;//�J�[�\�[���ʒu
	int curPos2;//�J�[�\�[���ʒu

	//�^�C�g���w�i
	LPDIRECT3DTEXTURE9 titlePic;
	MySpriteObject* titleSprite;
	//�{�^���p(GameStart)
	LPDIRECT3DTEXTURE9 startBtnPic;
	MySpriteObject* startBtn;
	//�{�^���p(Continue)
	LPDIRECT3DTEXTURE9 continueBtnPic;
	MySpriteObject* continueBtn;
	//�{�^���p(Config)
	LPDIRECT3DTEXTURE9 configBtnPic;
	MySpriteObject* configBtn;
	//�{�^���p(GameEnd)
	LPDIRECT3DTEXTURE9 endBtnPic;
	MySpriteObject* endBtn;
	//�J�[�\�[���p
	LPDIRECT3DTEXTURE9 selectPic;
	MySpriteObject* selectCur;


	//=============================
	//�R���t�B�O���
	//=============================

	//�w�i
	LPDIRECT3DTEXTURE9 confPic;
	MySpriteObject* confSprite;
	
	//=============================
	//�t�F�[�h�A�E�g
	//=============================
	LPDIRECT3DTEXTURE9 blackPic;
	MySpriteObject* blackOut;

	//=============================
	//�Z���N�g���
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