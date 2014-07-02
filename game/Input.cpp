#include "Input.h"
#include "Common.h"
#include "Error.h"

KeyMessage::KeyMessage()
:btn(KEY_NONE),rightleft(0),updown(0),zrightleft(0),zupdown(0),cmd(0)
{
}
KeyMessage::~KeyMessage(){
}


Input::Input()
:pKeyboard(NULL),pMouse(NULL)
{
	for (int i=0;i<INPUT_DEVICE_NUM; i++){
		pJoyStick[i] = NULL;
	}

	for (int i=0;i<INPUT_COUNT; i++){
		InputHistory[i]=0;
	}

	JoyStickCount = 0;
}
Input::~Input(){
}
HRESULT Input::InitInputDevice( HWND hWnd, HINSTANCE hInstance ){

	this->hWnd=hWnd;
	HRESULT		hr = S_OK;

	// DirectInputオブジェクトの生成
	if( FAILED( hr = DirectInput8Create( hInstance , DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDirectInput, NULL ) ) )
	{ return ( hr ); }

	if( FAILED( hr = SetupKeybord()) ){ return ( hr ); }
	if( FAILED( hr = SetupJoyStick()) ){ return ( hr ); }
	if( FAILED( hr = SetupMouse()) ){ return ( hr ); }
	return (hr);
}

HRESULT Input::SetupKeybord()
{
	HRESULT		hr;

	if( FAILED( hr = pDirectInput->CreateDevice( GUID_SysKeyboard, &pKeyboard, NULL ) ) )
	{ return ( hr ); }

	if( pKeyboard == NULL )
	{
		DisplayErrMsg("キーボードを発見できませんでした。");
		return( S_OK );
	}

	// DirectInputから受け取るデータフォーマットをキーボードに設定する
	if( FAILED( hr = pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )// c_dfDIKeyboardは、DirectInputが用意したグローバル変数
	{ return ( hr ); }

	// 協調レベルをセットする。
	//（フォアグランド・直接入力・協調モード・Windowsキー無効に固定）
	if( FAILED(hr = pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)) )
	{ return ( hr ); }

	// キーボードの入力を有効にする。
	pKeyboard->Acquire();

	return( S_OK );
}


HRESULT Input::SetupJoyStick()
{
	HRESULT		hr;

	if( FAILED( hr = pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL , EnumJoysticksCallback, (void*)this , DIEDFL_ATTACHEDONLY ) ) )
	{ return ( hr );}

	if( NULL == pJoyStick[0] )
	{
		DisplayErrMsg("ジョイスティックが発見できませんでした。");
		return( S_OK );
	}

	for(size_t i=0;i<JoyStickCount;++i){

		if( FAILED( hr = pJoyStick[i]->SetDataFormat( &c_dfDIJoystick2 ) ) )
		{ return ( hr ); }

		if( FAILED( hr = pJoyStick[i]->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
		{ return ( hr ); }

		if( FAILED( hr = pJoyStick[i]->EnumObjects( EnumObjectsCallback, (void*)pJoyStick[i], DIDFT_ALL ) ) )
		{ return ( hr );}

		// ジョイスティックの入力を有効にする。
		pJoyStick[i]->Acquire();
	}


	return (S_OK);
}

BOOL CALLBACK Input::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, void* pContext )
{
	Input* pThis = (Input*)pContext;
	HRESULT hr;

    // 列挙されたジョイスティックへのインタフェイスを獲得する
	// 複数のジョイスティックを認識させたい場合は
	// その数だけLPDIRECTINPUTDEVICE8型の変数を用意しておく
	hr = pThis->pDirectInput->CreateDevice( pdidInstance->guidInstance, &(pThis->pJoyStick[pThis->JoyStickCount]), NULL );

	// 失敗していた場合、そのジョイスティックは使えない。
	// ディバイスを列挙している途中でユーザーがジョイスティックを外した可能性が高い
	// ここでは、ジョイスティックが一つ見つかればそれでいいので、失敗したときに限って再度検索するようにしている。
    if( SUCCEEDED(hr) ) {
		if(++(pThis->JoyStickCount) >= INPUT_DEVICE_NUM){
			return ( DIENUM_STOP );
		}
	}

	return ( DIENUM_CONTINUE );
}

BOOL CALLBACK Input::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,void* pContext )
{
	LPDIRECTINPUTDEVICE8 pJoyStick = (LPDIRECTINPUTDEVICE8)pContext;

	static int nSliderCount = 0;  // 帰ってきたスライダコントローラの数を表す
	static int nPOVCount = 0;     // 帰ってきた方向コントローラの数を表す

	// 列挙されたオブジェクトが角度コントローラだった場合
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow        = DIPH_BYID;
		diprg.diph.dwObj        = pdidoi->dwType;
		diprg.lMin              = -JS_RESO;				// 角度ディバイスの最小値
		diprg.lMax              = JS_RESO;					// 角度ディバイスの最大値
   
//		MessageBox( NULL, "絶対値か相対値を出力するディバイスを確認しました。\n最大値、最小値を設定します。", kWINDOW_TITLE , MB_ICONEXCLAMATION | MB_OK );

      
		if( FAILED( pJoyStick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) { return( DIENUM_STOP ); }
	}
	return( DIENUM_CONTINUE );
}

HRESULT Input::SetupMouse()
{
	return (S_OK);
}

KeyMessage Input::GetKeybordState()
{
	KeyMessage	result;
	HRESULT		hr;
	BYTE		diks[256];					// DirectInputのキーボードステータスバッファ

	// キーボードディバイスが無い場合は、滑らかに去る。
	if( NULL == pKeyboard ) { return ( result ); }

	// 入力ディバイスの状態を確保し、配列におさめる。
	ZeroMemory( diks, sizeof(diks) );
	hr = pKeyboard->GetDeviceState( sizeof(diks), diks );
	if( FAILED(hr) )
	{
		// もし入力が失われていた場合、確保し、維持を試みる。
		hr = pKeyboard->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = pKeyboard->Acquire(); }

		// エラー時は帰る。
		if( FAILED(hr) ) {return ( result ); }
	}

	//======================================================================
	//キーボードからの入力による
	if((diks[0xcb] & 0x80)||(diks[0x4b] & 0x80 )){ result.btn |= KEY_LEFT;	result.rightleft	-=	1000.0f;}	//左
	if((diks[0xcd] & 0x80)||(diks[0x4d] & 0x80 )){ result.btn |= KEY_RIGHT;	result.rightleft	+=	1000.0f;}	//右
	if((diks[0xc8] & 0x80)||(diks[0x48] & 0x80 )){ result.btn |= KEY_UP;	result.updown		-=	1000.0f;}	//上
	if((diks[0xd0] & 0x80)||(diks[0x50] & 0x80 )){ result.btn |= KEY_DOWN;	result.updown		+=	1000.0f;}	//下
	//======================================================================

	if(diks[0x2c] & 0x80){ result.btn |= KEY_A; }		// Z
	if(diks[0x2d] & 0x80){ result.btn |= KEY_B; }		// X
	if(diks[0x2e] & 0x80){ result.btn |= KEY_X; }		// C
	if(diks[0x39] & 0x80){ result.btn |= KEY_SELECT; }	// SPACE
	if(diks[0x1c] & 0x80){ result.btn |= KEY_START; }	// enter

	return( result );
}

HRESULT Input::GetJoystickState(KeyMessage *result, size_t JoyStickNum )
{
	HRESULT		hr = S_OK;
//	TCHAR		strText[512];	// ディバイスの状態を取得するバッファ（特にボタン）
	DIJOYSTATE2	pJoyStickState;	// ジョイスティックの状態を取得する
//	TCHAR*		str;

	if( NULL == pJoyStick ) 
	{ 
		return ( S_OK );
	}

	//ジョイスティックの接続本数が番号より少なければNG
	if( JoyStickNum >= JoyStickCount ) 
	{ 
		return ( S_OK );
	}
	// ディバイスの現在の状況を取得する
	hr = pJoyStick[JoyStickNum]->Poll(); 
	if( FAILED(hr) )  
	{
		// DirectInputはPollメソッドが呼び出された時点での入力ストリームの状態を伝えるが
		// 入力ディバイスが何らかの原因で失われたとしてもそれをメソッド返すエラー以外で知ることは出来ない。
		// また、ディバイスをリセットする手段も無い。
		// そのため、ディバイスを再取得することを試みる。
//		if(  hr == DIERR_NOTACQUIRED){return S_OK;}
		hr = pJoyStick[JoyStickNum]->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = pJoyStick[JoyStickNum]->Acquire(); }

		// それ以外のエラーは、ほかのアプリケーションがフォアグランドになっているか
		// アプリケーション自体が最小化されてデータが取得できない場合がほとんど。
		// こうした場合は、何もせずに帰って後で再取得を試みる
		return ( S_OK ); 
	}

	// ジョイスティックの状態を取得する
	// ここで取得できる状態は、先のPoll()メソッドを呼び出した時点のもの
	// GetDeviceStateの呼び出しが遅れれば、当然それだけ古い情報になってしまう
	// ここがエラーを返す場合、ジョイスティックが引き抜かれた可能性が高い
	if( FAILED( hr = pJoyStick[JoyStickNum]->GetDeviceState( sizeof(DIJOYSTATE2), &pJoyStickState ) ) ){ return ( hr );}

    // 今回のサンプルで必要なのは、いわゆる方向ボタンといくつかのボタンだけなので、それ以外の情報は無視する

    // 方向ボタンの検出（Z軸は見ていない）
	if( pJoyStickState.lX > JS_DEAD ){
		result->btn |= KEY_RIGHT;
	}else if(pJoyStickState.lX < -JS_DEAD ){
		result->btn |= KEY_LEFT;
	}

	if( pJoyStickState.lY > JS_DEAD ){
		result->btn |= KEY_DOWN;
	}else if(pJoyStickState.lY < -JS_DEAD ){ 
		result->btn |= KEY_UP; 
	}

	if( pJoyStickState.lX != 0 ){
		result->rightleft += (float)pJoyStickState.lX;
	}
	if( pJoyStickState.lY != 0 ){
		result->updown += (float)pJoyStickState.lY;
	}

	if( pJoyStickState.lRz != 0 ){
		result->zupdown += (float)pJoyStickState.lRz;
	}
	if( pJoyStickState.lZ != 0 ){
		result->zrightleft += (float)pJoyStickState.lZ;
	}


   
	// どのボタンが押されたか確認する
	// rgbButtonsは押されたボタンに対応した配列の最上位ビットが1になる
	if(pJoyStickState.rgbButtons[0x00] & 0x80){ result->btn |= KEY_X; }
	if(pJoyStickState.rgbButtons[0x01] & 0x80){ result->btn |= KEY_A; }
	if(pJoyStickState.rgbButtons[0x02] & 0x80){ result->btn |= KEY_B; }
	if(pJoyStickState.rgbButtons[0x03] & 0x80){ result->btn |= KEY_Y; }
	if(pJoyStickState.rgbButtons[0x04] & 0x80){ result->btn |= KEY_S; }
	if(pJoyStickState.rgbButtons[0x05] & 0x80){ result->btn |= KEY_D; }
	if(pJoyStickState.rgbButtons[0x06] & 0x80){ result->btn |= KEY_L; }
	if(pJoyStickState.rgbButtons[0x07] & 0x80){ result->btn |= KEY_R; }
	if(pJoyStickState.rgbButtons[0x08] & 0x80){ result->btn |= KEY_START; }
	if(pJoyStickState.rgbButtons[0x09] & 0x80){ result->btn |= KEY_SELECT; }

//	MyOutputDebugString( _T("<Zstick:%f, %f>\n"),result->zupdown,result->zrightleft);

	return ( hr );
}

HRESULT Input::InitKeyboard(KeyMessage *result)
{
	return (S_OK);
}


HRESULT Input::GetCommand(KeyMessage* result){

	//入力されていないキーをチェックする
	if(!(result->btn&KEY_LEFT)){	result->btn |= KEY_LEFTn;}
	if(!(result->btn&KEY_RIGHT)){	result->btn |= KEY_RIGHTn;}
	if(!(result->btn&KEY_UP)){		result->btn |= KEY_UPn;}
	if(!(result->btn&KEY_DOWN)){	result->btn |= KEY_DOWNn;}
	if(!(result->btn&KEY_A)){		result->btn |= KEY_An; }
	if(!(result->btn&KEY_B)){		result->btn |= KEY_Bn; }
	if(!(result->btn&KEY_X)){		result->btn |= KEY_Xn; }
	if(!(result->btn&KEY_Y)){		result->btn |= KEY_Yn; }
	if(!(result->btn&KEY_S)){		result->btn |= KEY_Sn; }
	if(!(result->btn&KEY_D)){		result->btn |= KEY_Dn; }
	if(!(result->btn&KEY_R)){		result->btn |= KEY_Rn; }
	if(!(result->btn&KEY_L)){		result->btn |= KEY_Ln; }
	if(!(result->btn&KEY_A)){		result->btn |= KEY_An; }
	if(!(result->btn&KEY_START)){	result->btn |= KEY_STARTn; }
	if(!(result->btn&KEY_SELECT)){	result->btn |= KEY_SELECTn; }

	//履歴の保存
	for (int i=INPUT_COUNT-1; i>0; i--) InputHistory[i]=InputHistory[i-1];
	InputHistory[0]=result->btn;

	//コマンドチェック
	if(CommandCheck(CMD_LEFT	,result)){result->cmd |=NEW_LEFT;	}
	if(CommandCheck(CMD_RIGHT	,result)){result->cmd |=NEW_RIGHT;	}
	if(CommandCheck(CMD_UP		,result)){result->cmd |=NEW_UP;		}
	if(CommandCheck(CMD_DOWN	,result)){result->cmd |=NEW_DOWN;	}
	if(CommandCheck(CMD_A		,result)){result->cmd |=NEW_A;		}
	if(CommandCheck(CMD_B		,result)){result->cmd |=NEW_B;		}
	if(CommandCheck(CMD_X		,result)){result->cmd |=NEW_X;		}
	if(CommandCheck(CMD_Y		,result)){result->cmd |=NEW_Y;		}
	if(CommandCheck(CMD_S		,result)){result->cmd |=NEW_S;		}
	if(CommandCheck(CMD_D		,result)){result->cmd |=NEW_D;		}
	if(CommandCheck(CMD_L		,result)){result->cmd |=NEW_L;		}
	if(CommandCheck(CMD_R		,result)){result->cmd |=NEW_R;		}
	if(CommandCheck(CMD_SELECT	,result)){result->cmd |=NEW_SELECT;	}
	if(CommandCheck(CMD_START	,result)){result->cmd |=NEW_START;	}

	//MyOutputDebugString( _T("<CMD:%lx, %lx, %lx>\n"),(long)result->cmd,(long)result->btn,(long)InputHistory[INPUT_COUNT-1]);

	return (S_OK);
}
bool Input::CommandCheck(const unsigned long *command,KeyMessage* result){
	//コマンドの最後を調べる
	int i=0, j=0;
	while (!(command[i]==KEY_ERR)) i++;

	//コマンドの入力時間を取得する
	int time=command[0];

	//入力履歴を調べて、
	//コマンドが正しく入力されていればTrue
	for (i--; i>=1; i--) {
		int input=command[i];//一つ一つコマンドをチェックする
		while (j<INPUT_COUNT && (InputHistory[j]&input)!=input){j++;}//履歴とコマンドを比較し、違う間ループ
		if (j>=time || j==INPUT_COUNT) 
			return false;//制限時間をオーバーか履歴の末尾へ来た
	}
	return true;
}

void Input::CleanUpDirectInput( void )
{
	// DirectInputでキーボード、ジョイスティック、マウスのいずれかが使用中だった場合、使用を終了する。
	if( pKeyboard ){ pKeyboard->Unacquire(); }    
	SAFE_RELEASE( pKeyboard );

	for(size_t i = 0;i < JoyStickCount;++i){
		if( pJoyStick ){ pJoyStick[i]->Unacquire(); }
		SAFE_RELEASE( pJoyStick[i] );
	}

	if( pMouse ){ pMouse->Unacquire(); }
	SAFE_RELEASE( pMouse );

	// DirectInputの解放。
	SAFE_RELEASE( pDirectInput );
}