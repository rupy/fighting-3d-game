#include "Window.h"
#include "Common.h"
//=============================================================
// コンストラクタ（アプリケーションの生成）
//=============================================================
App::App(HINSTANCE hInstance, bool isFullScreen)
:game(NULL),
hInstance(hInstance),
isFullScreen(isFullScreen),
lostFocus(FALSE),
isPause(FALSE)
{
	// 多重起動のチェック
	hMutex = CreateMutex(NULL, FALSE, WINDOW_TITLE);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(hMutex){CloseHandle(hMutex);}
		throw "このアプリケーションは多重起動できません。";
	}

	//ウインドウクラスの登録
	if(FAILED(RegistWndClass())){
		throw "ウインドウクラスの登録に失敗しました";
	}

	//ウインドウの生成
	if(FAILED(CreateMainWnd())){
		throw "ウインドウクラスの登録に失敗しました";
	}

	//メニューバーの生成
	if(!isFullScreen){
		CreateMenuBar();
	}

	//DIRECTINPUTの初期化
	//(Direct3D初期化の前にやっておかなければエラーダイアログが見れなくなる)
	if(FAILED(input.InitInputDevice(hWnd,hInstance))){
		throw "DirectInputの初期化に失敗しました";
	}


	//DIRECT3Dの初期化
	if(FAILED(renderer.Initialize(hWnd,isFullScreen,WINDOW_WIDTH,WINDOW_HEIGHT))){
		throw "DirectXの初期化に失敗しました";
	}

	//DIRECTSOUNDの初期化
	if(FAILED(sound.InitDirectSound(hWnd))){
		throw "DirectSoundの初期化に失敗しました";
	}

	game = new GameBase(
		renderer.GetDevice(),
		renderer.GetPParam(),
		renderer.GetSprite(),
		&input,
		&sound);

}
//=============================================================
// デストラクタ
//=============================================================
App::~App(){
	
	SAFE_DELETE(game);
	//ミューテックスの解放
	if(hMutex){CloseHandle(hMutex);}
}


//=============================================================
// ウインドウクラスの登録
//=============================================================
HRESULT App::RegistWndClass(){

	wc.cbSize=sizeof(WNDCLASSEX);					// この構造体のサイズ
	wc.style=NULL;									// ウインドウのスタイル(default)
	wc.lpfnWndProc=DummyProc;						// ダミーのウインドウプロシージャの登録
	wc.cbWndExtra=0;								// 通常は使わないので常に0
	wc.cbClsExtra=0;								// 通常は使わないので常に0
	wc.hInstance=hInstance;							// インスタンスへのハンドル
	wc.hIcon=NULL;									// アイコン（なし）
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);			// カーソルの形
	wc.hbrBackground=NULL;							// 背景なし
	wc.lpszMenuName=NULL;							// メニューなし
	wc.lpszClassName=WINDOW_TITLE;					// クラス名の指定
	wc.hIconSm=NULL;								// 小アイコン（なし）

	if( !RegisterClassEx( &wc ) ){
		return(E_FAIL);
	}
	
	return(S_OK);
}
//=============================================================
// ウインドウの生成
//=============================================================
HRESULT App::CreateMainWnd(){

	//最大化ボタンを持たない・境界変更のできないwindow
	DWORD	dwWindowStyle= ~(WS_MAXIMIZEBOX | WS_THICKFRAME) & WS_OVERLAPPEDWINDOW;

	hWnd = CreateWindow(	WINDOW_TITLE, WINDOW_TITLE, dwWindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL );

	//静的メンバ関数のウインドウプロシージャにthisポインタを渡すため。
	SetWindowLong( hWnd, GWL_USERDATA, (LONG)this );
	//ウインドプロシージャのすり替え（thisポインタが通るまでは機能しないため）
	SetWindowLong( hWnd, GWL_WNDPROC, (LONG)WindowProc);

	// ウィンドウサイズを再設定する
	RECT rect;
	int ww, wh;
	int cw, ch;

	// ウインドウ全体の横幅の幅を計算
	GetWindowRect(hWnd, &rect);		// ウインドウ全体のサイズ取得
	ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算

	// クライアント領域の外の幅を計算
	GetClientRect(hWnd, &rect);		// クライアント部分のサイズの取得
	cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
	ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

	ww = ww - cw;					// クライアント領域以外に必要な幅
	wh = wh - ch;					// クライアント領域以外に必要な高さ

	// ウィンドウサイズの再計算
	ww = WINDOW_WIDTH + ww;			// 必要なウインドウの幅
	wh = WINDOW_HEIGHT + wh;		// 必要なウインドウの高さ

	// ウインドウサイズの再設定
	SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	// ウィンドウの表示
    ShowWindow(hWnd, SW_SHOWDEFAULT);

	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);


	return(S_OK);
}
//=============================================================
// メニューバーの生成
//=============================================================
void App::CreateMenuBar(){
	HMENU hMenu;
	HMENU hSubMenu;
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);

	hMenu = CreateMenu();
	hSubMenu = CreatePopupMenu();
	//メニューの追加
	//ゲーム(&G)
	//	>新規作成(&N)
	//	>設定(&S)
	//	>終了(&X)
	//フルスクリーン(&F)
	//バージョン情報(&H)

	mii.dwTypeData = _T("新規作成(&N)");
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.fType = MFT_STRING;
	mii.wID = IDM_NEW;
	InsertMenuItem(hSubMenu,0,TRUE,&mii);

	mii.dwTypeData = _T("設定(&S)");
	mii.wID = IDM_SETTING;
	InsertMenuItem(hSubMenu,1,TRUE,&mii);

	mii.dwTypeData = _T("終了(&X)");
	mii.wID = IDM_CLOSE;
	InsertMenuItem(hSubMenu,2,TRUE,&mii);


	mii.fMask = MIIM_TYPE|MIIM_SUBMENU;
	mii.dwTypeData = _T("ゲーム(&G)");
	mii.hSubMenu = hSubMenu;
	InsertMenuItem(hMenu,0,TRUE,&mii);

	mii.dwTypeData = _T("フルスクリーン(&F)");
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.hSubMenu = NULL;
	mii.wID = IDM_FULLSCREEN;
	InsertMenuItem(hMenu,1,TRUE,&mii);

	mii.dwTypeData = _T("バージョン情報(&H)");
	mii.wID = IDM_VERSION;
	InsertMenuItem(hMenu,2,TRUE,&mii);

	SetMenu(hWnd,hMenu);//メニューを設定
}
//=============================================================
//　メッセージループ
//=============================================================
HRESULT App::MsgLoop()
{
	// メッセージループ
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	DWORD dwCurTime=0;
	DWORD dwLastTime=0;
	BOOL bMove=TRUE;
		

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage (&msg,NULL,0,0,PM_NOREMOVE)) 
		{
			if( !GetMessage (&msg,NULL,0,0) ){ msg.message = WM_QUIT; }	// 終了
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{

			if(lostFocus||isPause){
				game->Display();
				continue;
			}
			Sleep(1);

			//移動と描画
			if(bMove){
				bMove=FALSE;
				if(game->Run()){
					PostQuitMessage(0);
					continue;
				}
				game->Draw();
			}

			timeBeginPeriod(1);
			dwCurTime = timeGetTime();
			timeEndPeriod(1);

			//表示
			if (dwCurTime - dwLastTime>=FLAME_RATE) {
				if(FAILED(game->Display())){
					MessageBox( NULL, "予期しないデバイスロストが起きました。", WINDOW_TITLE, MB_OK|MB_ICONHAND );
					return E_FAIL;
				}
				dwLastTime = dwCurTime;
				bMove=TRUE;
			}else{
				if(dwCurTime < dwLastTime){
					dwLastTime=dwCurTime;
				}
			}
		}
	}
	return(S_OK);

}
//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK App::DummyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//-------------------------------------------------------------
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK App::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//thisポインタを受け取り、ウインドプロシージャ内で使えるようにする
	App* pThis = (App*)GetWindowLong( hWnd, GWL_USERDATA );

	switch (msg) {
	//ウインドウが閉じられた
	case WM_CLOSE:
		PostQuitMessage(0);			// アプリケーションを終了する
		break;
	//キーが押された
	case WM_KEYDOWN:
		//ESCキー
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);		// アプリケーションを終了する
		}else
		//SPACEキー
		if (wParam == VK_SPACE) {
			pThis->ToggleFullScreen(FALSE);
		}else 
		//F1キー
		if (wParam == VK_F1 ) {
			pThis->isPause = !pThis->isPause;
		}
		break;
	//フォーカスが戻った
	case WM_SETFOCUS:
		pThis->lostFocus = FALSE;
		break;
	//フォーカスを失った
	case WM_KILLFOCUS:
		pThis->lostFocus = TRUE;
		break;
	//メニューバー関連
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDM_CLOSE:
			PostQuitMessage(0);		// アプリケーションを終了する
			break;
		case IDM_VERSION:
			MessageBox( NULL, "バージョン１です。\n", "バージョン情報", MB_OK | MB_SETFOREGROUND|MB_TOPMOST );
			break;
		case IDM_FULLSCREEN:
			pThis->ToggleFullScreen(TRUE);
			break;
		default:
			break;
		}
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//=============================================================
// アプリケーションの実行
//=============================================================
HRESULT App::Run(){

	if(FAILED(MsgLoop())){
		return(E_FAIL);
	}
	return(S_OK);
}

//=============================================================
// フルスクリーンの切り替え
//=============================================================
HRESULT App::ToggleFullScreen(bool isFullScreen){

  	this->isFullScreen = isFullScreen;

	D3DPRESENT_PARAMETERS* pD3DPP=renderer.GetPParam();
	if(isFullScreen){
		SetMenu(hWnd,NULL);

		pD3DPP->Windowed				= FALSE;			// フルスクリーン表示の指定
		pD3DPP->BackBufferWidth			= WINDOW_WIDTH;		// フルスクリーン時の横幅
		pD3DPP->BackBufferHeight		= WINDOW_HEIGHT;		// フルスクリーン時の縦幅
	}else{
		CreateMenuBar();
		pD3DPP->Windowed				= TRUE;			// フルスクリーン表示の指定	
	}

	game->DeviceRecet();

	return S_OK;
}