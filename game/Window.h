#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>
#include "Renderer.h"
#include "Game.h"
#include "Input.h"
#include "Sound.h"

#define SLEEP_TIME 8
#define FPS 60.0f
#define FLAME_RATE 1000.0f/FPS

class App{

private:
	HANDLE		hMutex;			//ミューテックス
	WNDCLASSEX	wc;				//ウインドウクラス
	HINSTANCE	hInstance;		//インスタンスハンドル
	HWND		hWnd;			//ウインドハンドル
	bool		isFullScreen;	//フルスクリーンON/OFF
	bool		lostFocus;		//ウインドウにフォーカスがあるかどうか
	bool		isPause;		//ゲームの中断

	Renderer	renderer;		//Direct3Dデバイスなどの生成、保持クラス
	GameBase*	game;			//ゲームクラス（ゲームの本体）
	Input		input;			//DirectInputデバイスの生成、入力管理
	Sound		sound;			//DirectSoundデバイスの生成、サウンド管理


public:


	//-------------------------------------------------------------
	// コンストラクタ（アプリケーションの生成）
	// 引数
	//		hInstance	: WinMain関数から受け取ったインスタンスハンドル
	//		isFullScreen: フルスクリーンON/OFF
	//-------------------------------------------------------------
	App(HINSTANCE hInstance, bool isFullScreen);
	//-------------------------------------------------------------
	// デストラクタ（ミューテックスの開放など）
	// 引数
	//		hInstance	: WinMain関数から受け取ったインスタンスハンドル
	//		isFullScreen: フルスクリーンON/OFF
	//-------------------------------------------------------------
	~App();

	//アプリケーションの実行
	HRESULT Run();

private:
	//ウインドウクラスの登録
	HRESULT RegistWndClass();
	//ウインドウの生成
	HRESULT CreateMainWnd();
	//メニューバーの生成
	void CreateMenuBar();
	//メッセージループ
	HRESULT MsgLoop();

	//ウインドウプロシージャ
	static LRESULT CALLBACK DummyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//ダミー
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//フルスクリーン切り替え
	HRESULT ToggleFullScreen(bool isFullScreen);
};

//メニューバー選択のメッセージ
enum MenuBarMsgEnum{
	IDM_NEW = 40001,//新規作成
	IDM_SETTING,	//設定
	IDM_CLOSE,		//終了
	IDM_FULLSCREEN,	//フルスクリーン切り替え
	IDM_VERSION		//バージョン情報
};

#endif	/*__WINDOW_H__*/