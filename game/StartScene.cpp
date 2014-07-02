#include "StartScene.h"

StartScene::StartScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm)
:SceneBase(device,sprite,sound,sm),
mv(NULL),
titlePic(NULL),titleSprite(NULL),
startBtnPic(NULL),startBtn(NULL),
continueBtnPic(NULL),continueBtn(NULL),
configBtnPic(NULL),configBtn(NULL),
endBtn(NULL),endBtnPic(NULL),
selectPic(NULL),selectCur(NULL),blackPic(NULL),blackOut(NULL),confSprite(NULL),confPic(NULL)
{
}

StartScene::~StartScene(){
	SAFE_DELETE(light);
	SAFE_DELETE(mv);
	SAFE_DELETE(titleSprite);
	SAFE_DELETE(startBtn);
	SAFE_DELETE(continueBtn);
	SAFE_DELETE(configBtn);
	SAFE_DELETE(endBtn);
	SAFE_DELETE(selectCur);
	SAFE_DELETE(blackOut);
	SAFE_DELETE(confSprite);
	SAFE_DELETE(sceneSelectSprite);
	SAFE_DELETE(selectCur2);


	SAFE_RELEASE(titlePic);
	SAFE_RELEASE(startBtnPic);
	SAFE_RELEASE(continueBtnPic);
	SAFE_RELEASE(configBtnPic);
	SAFE_RELEASE(endBtnPic);
	SAFE_RELEASE(selectPic);
	SAFE_RELEASE(confPic);
	SAFE_RELEASE(blackPic);
	SAFE_RELEASE(sceneSelectPic);
}

HRESULT StartScene::Initialize(){

	//ライトの初期化
	//light = new Light( D3DLIGHT_DIRECTIONAL, 0, D3DXVECTOR3(100.0f,100.0f,100.0f));

	//ムービーの読み込み
	if(FAILED(SetupMovies())){
		return E_FAIL;
	}
	//テクスチャ読み込み
	if(FAILED(SetupTextures())){
		return (E_FAIL);
	}
	//オブジェクトの初期化
	if(FAILED(SetupObjects())){
		return E_FAIL;
	}
	//準備
	if(FAILED(Prepare())){
		return E_FAIL;
	}

	return (S_OK);
}
//=============================================================
// モデルの読み込み
//=============================================================
HRESULT StartScene::SetupModels(){

	return(S_OK);
}
//=============================================================
// テクスチャの読み込み
//=============================================================
HRESULT StartScene::SetupTextures(){

	//=========================================================
	//タイトル画面用
	//=========================================================
	if(FAILED(D3DXCreateTextureFromFileEx( pDevice, "picture/title.jpg",800,600,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&titlePic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	if(FAILED(D3DXCreateTextureFromFileEx( pDevice, "picture/black.jpg",800,600,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&blackPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	if(FAILED(D3DXCreateTextureFromFile( pDevice, "picture/btn_start.jpg",&startBtnPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile( pDevice, "picture/btn_load.jpg",&continueBtnPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile( pDevice, "picture/btn_opt.jpg",&configBtnPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	if(FAILED(D3DXCreateTextureFromFile( pDevice, "picture/btn_end.jpg",&endBtnPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	if(FAILED(D3DXCreateTextureFromFile( pDevice, "picture/select.png",&selectPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	//=========================================================
	//コンフィグ画面用
	//=========================================================
	if(FAILED(D3DXCreateTextureFromFileEx( pDevice, "picture/config.jpg",800,600,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&confPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	//=========================================================
	//選択画面用
	//=========================================================
	if(FAILED(D3DXCreateTextureFromFileEx( pDevice, "picture/scene_select.jpg",800,600,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&sceneSelectPic))){
		MessageBox( NULL, "テクスチャの読み込みに失敗しました", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}

	HANDLE hSearch;
	WIN32_FIND_DATA fd;
	string filename;
	HWND hWnd=FindWindow(WINDOW_TITLE,NULL);
		
	int count = 0;//ファイルの数
	hSearch = FindFirstFile( _T("models/map/*.x"), &fd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	while( TRUE )
	{
		// 列挙されたファイル名を書き出す
		filename = fd.cFileName; // ファイル名をバッファにコピー


		files.push_back(
			StringObject(
				D3DXVECTOR3(float(LEFT + count/col_num * raw_space),
				float((WINDOW_HEIGHT-TOP) - count%col_num*col_space),
				0
				),
			780,25,0,D3DXCOLOR(255,255,255,255),1,0,5,0,2,const_cast<char*>(filename.c_str())));

		filenames.push_back(filename);

		filename.clear();
		++count;
		if( !FindNextFile( hSearch, &fd ) )
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				break;
			}
			else
			{
				MessageBox( hWnd, _T("マップファイルの列挙に失敗しました"), _T("ファイルの列挙エラー"), MB_OK );
				break;
			}
		}
	}

	FindClose( hSearch );


	return(S_OK);
}
HRESULT StartScene::SetupObjects(){

	//=============================
	//タイトル画面用
	//=============================
	//スプライト準備
	titleSprite = new MySpriteObject(800,600,CPF_LEFT_BOTTOM,titlePic);
	startBtn = new MySpriteObject(128,32,CPF_CENTER,startBtnPic,D3DXVECTOR3(400,246,0));
	continueBtn = new MySpriteObject(128,32,CPF_CENTER,continueBtnPic,D3DXVECTOR3(400,214,0));
	configBtn = new MySpriteObject(128,32,CPF_CENTER,configBtnPic,D3DXVECTOR3(400,182,0));
	endBtn = new MySpriteObject(128,32,CPF_CENTER,endBtnPic,D3DXVECTOR3(400,150,0));
	selectCur = new MySpriteObject(32,32,CPF_CENTER,selectPic,D3DXVECTOR3(300,246,0));
	blackOut = new MySpriteObject(800,600,CPF_LEFT_BOTTOM,blackPic);

	//=============================
	//コンフィグ画面用
	//=============================
	//スプライト準備
	confSprite = new MySpriteObject(800,600,CPF_LEFT_BOTTOM,confPic);
	//=============================
	//セレクト画面用
	//=============================
	//スプライト準備
	sceneSelectSprite = new MySpriteObject(800,600,CPF_LEFT_BOTTOM,sceneSelectPic);
	selectCur2 = new MySpriteObject(32,32,CPF_LEFT_TOP,selectPic,D3DXVECTOR3(5.0f,WINDOW_HEIGHT-TOP,0));

	return S_OK;
}
HRESULT StartScene::SetupMovies(){

	mv = new Movie();
	if(FAILED(mv->InitMovie(L"movie/logo.mpg"))){
		DisplayErrMsg("ムービーが開けませんでした");	
		return (E_FAIL);
	}
	return S_OK;
}

HRESULT StartScene::Prepare(){

	//カーソルの初期化
	curPos = 0;
	curPos2 = 0;

	//セーブファイルが存在するときカーソルを下へ
	if(pSM->CheckSaveFile()){
		curPos++;
		selectCur->Pos.y-=CURSOR_MOVE;
	}

	canClear = true;

	sceneMode = START_MOVIE;
	return S_OK;
}

SceneBase* StartScene::Move(KeyMessage* keyMessage){
	switch(sceneMode){
		//=============================
		//ムービー
		//=============================
		case START_MOVIE:
			canClear = false;
			//ムービーを再生する
			mv->PlayMovie();
			sceneMode = PLAY_LOGO_MOVIE;
			break;
		case PLAY_LOGO_MOVIE:
			//ムービーの終了監査
			if(mv){
				if(!mv->CheckNowPlaying()||keyMessage->btn & KEY_START){
					sceneMode = MOVIE_END;
				}
			}
			break;
		case MOVIE_END:
			//ムービーの終了
			mv->StopMovie();
			SAFE_DELETE(mv);
			canClear = true;
			sceneMode = TITLE;
			break;
		//=============================
		//タイトル画面
		//=============================
		case TITLE:

			if(keyMessage->cmd & (NEW_START|NEW_A)&& curPos == GAME_START){
				sceneMode = SCENE_SELECT;
			}
			if(keyMessage->cmd & (NEW_START|NEW_A)&& curPos == CONTINUE){
				sceneMode = GAME_CONFIG;
			}
			if(keyMessage->cmd & (NEW_START|NEW_A)&& curPos == CONFIG){
				sceneMode = GAME_CONFIG;
			}
			if(keyMessage->cmd & (NEW_START|NEW_A)&& curPos == GAME_END){
				PostQuitMessage(0);
				return NULL;
			}
			if(keyMessage->cmd & NEW_UP){
				if(curPos>0){
					curPos--;
					selectCur->Pos.y+=CURSOR_MOVE;
				}
			}
			if(keyMessage->cmd & NEW_DOWN){
				if(curPos<GAME_END){
					curPos++;
					selectCur->Pos.y-=CURSOR_MOVE;
				}
			}
//			MyOutputDebugString( _T("<POS:%f,%d>\n"),selectCur->Pos.y,curPos);

			break;
		//=============================
		//選択画面
		//=============================
		case SCENE_SELECT:
			for(size_t i=0;i<files.size();i++){
				files[i].fontColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			}

			if(keyMessage->cmd & (NEW_START|NEW_A)){
				sceneMode = CHANGE_NEXT;
			}
			if(keyMessage->cmd & NEW_UP){
				if(curPos2>0){
					curPos2--;
					if((curPos2 + 1)%col_num == 0){
						selectCur2->Pos.y-=CURSOR_MOVE*(col_num-1);
						selectCur2->Pos.x-=raw_space;
					}else{
						selectCur2->Pos.y+=CURSOR_MOVE;
					}
				}
			}
			if(keyMessage->cmd & NEW_DOWN){
				if(curPos2<(int)files.size()-1){
					curPos2++;
					if(curPos2%col_num == 0){
						selectCur2->Pos.y+=CURSOR_MOVE*(col_num-1);
						selectCur2->Pos.x+=raw_space;
					}else{
						selectCur2->Pos.y-=CURSOR_MOVE;
					}
				}
			}
			files[curPos2].fontColor = D3DXCOLOR(0.7f,0.3f,0.0f,1.0f);
			break;
		case CHANGE_NEXT:
				pSM->InitState();
				pSM->DataSave(0);
				pSM->state.file = "models/map/";
				pSM->state.file += filenames[curPos2];
				return new BeachScene(pDevice,pSprite,pSound,pSM);
			break;
		//=============================
		//コンフィグ画面用
		//=============================
		case GAME_CONFIG:
			if(keyMessage->cmd & NEW_B){
				sceneMode = TITLE;//タイトル画面へ戻る
			}
			break;
		default:
			break;
	}
	return 0;
}

void StartScene::Draw(){
	ClearFor2DDraw();

	size_t size;
	switch(sceneMode){
		//=============================
		//ムービー
		//=============================
		case START_MOVIE:
		case PLAY_LOGO_MOVIE:
		case MOVIE_END:
			break;
		//=============================
		//タイトル画面
		//=============================
		case TITLE:

			//pDevice->SetRenderState( D3DRS_LIGHTING, false );
			titleSprite->Appear();
			startBtn->Appear();
			continueBtn->Appear();
			configBtn->Appear();
			endBtn->Appear();
			selectCur->Appear();

//			break;
			//blackOut->Appear();
			break;
		//=============================
		//コンフィグ画面用
		//=============================
		case GAME_CONFIG:
			confSprite->Appear();
			break;
		case SCENE_SELECT:
		case CHANGE_NEXT:
			sceneSelectSprite->Appear();
			size = files.size();
			for(size_t i=0;i<size;i++){
				files[i].Appear();
			}
			selectCur2->Appear();
			break;
		default:
			break;
	}
}