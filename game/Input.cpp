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

	// DirectInput�I�u�W�F�N�g�̐���
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
		DisplayErrMsg("�L�[�{�[�h�𔭌��ł��܂���ł����B");
		return( S_OK );
	}

	// DirectInput����󂯎��f�[�^�t�H�[�}�b�g���L�[�{�[�h�ɐݒ肷��
	if( FAILED( hr = pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )// c_dfDIKeyboard�́ADirectInput���p�ӂ����O���[�o���ϐ�
	{ return ( hr ); }

	// �������x�����Z�b�g����B
	//�i�t�H�A�O�����h�E���ړ��́E�������[�h�EWindows�L�[�����ɌŒ�j
	if( FAILED(hr = pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)) )
	{ return ( hr ); }

	// �L�[�{�[�h�̓��͂�L���ɂ���B
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
		DisplayErrMsg("�W���C�X�e�B�b�N�������ł��܂���ł����B");
		return( S_OK );
	}

	for(size_t i=0;i<JoyStickCount;++i){

		if( FAILED( hr = pJoyStick[i]->SetDataFormat( &c_dfDIJoystick2 ) ) )
		{ return ( hr ); }

		if( FAILED( hr = pJoyStick[i]->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
		{ return ( hr ); }

		if( FAILED( hr = pJoyStick[i]->EnumObjects( EnumObjectsCallback, (void*)pJoyStick[i], DIDFT_ALL ) ) )
		{ return ( hr );}

		// �W���C�X�e�B�b�N�̓��͂�L���ɂ���B
		pJoyStick[i]->Acquire();
	}


	return (S_OK);
}

BOOL CALLBACK Input::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, void* pContext )
{
	Input* pThis = (Input*)pContext;
	HRESULT hr;

    // �񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�t�F�C�X���l������
	// �����̃W���C�X�e�B�b�N��F�����������ꍇ��
	// ���̐�����LPDIRECTINPUTDEVICE8�^�̕ϐ���p�ӂ��Ă���
	hr = pThis->pDirectInput->CreateDevice( pdidInstance->guidInstance, &(pThis->pJoyStick[pThis->JoyStickCount]), NULL );

	// ���s���Ă����ꍇ�A���̃W���C�X�e�B�b�N�͎g���Ȃ��B
	// �f�B�o�C�X��񋓂��Ă���r���Ń��[�U�[���W���C�X�e�B�b�N���O�����\��������
	// �����ł́A�W���C�X�e�B�b�N���������΂���ł����̂ŁA���s�����Ƃ��Ɍ����čēx��������悤�ɂ��Ă���B
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

	static int nSliderCount = 0;  // �A���Ă����X���C�_�R���g���[���̐���\��
	static int nPOVCount = 0;     // �A���Ă��������R���g���[���̐���\��

	// �񋓂��ꂽ�I�u�W�F�N�g���p�x�R���g���[���������ꍇ
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg; 
		diprg.diph.dwSize       = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow        = DIPH_BYID;
		diprg.diph.dwObj        = pdidoi->dwType;
		diprg.lMin              = -JS_RESO;				// �p�x�f�B�o�C�X�̍ŏ��l
		diprg.lMax              = JS_RESO;					// �p�x�f�B�o�C�X�̍ő�l
   
//		MessageBox( NULL, "��Βl�����Βl���o�͂���f�B�o�C�X���m�F���܂����B\n�ő�l�A�ŏ��l��ݒ肵�܂��B", kWINDOW_TITLE , MB_ICONEXCLAMATION | MB_OK );

      
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
	BYTE		diks[256];					// DirectInput�̃L�[�{�[�h�X�e�[�^�X�o�b�t�@

	// �L�[�{�[�h�f�B�o�C�X�������ꍇ�́A���炩�ɋ���B
	if( NULL == pKeyboard ) { return ( result ); }

	// ���̓f�B�o�C�X�̏�Ԃ��m�ۂ��A�z��ɂ����߂�B
	ZeroMemory( diks, sizeof(diks) );
	hr = pKeyboard->GetDeviceState( sizeof(diks), diks );
	if( FAILED(hr) )
	{
		// �������͂������Ă����ꍇ�A�m�ۂ��A�ێ������݂�B
		hr = pKeyboard->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = pKeyboard->Acquire(); }

		// �G���[���͋A��B
		if( FAILED(hr) ) {return ( result ); }
	}

	//======================================================================
	//�L�[�{�[�h����̓��͂ɂ��
	if((diks[0xcb] & 0x80)||(diks[0x4b] & 0x80 )){ result.btn |= KEY_LEFT;	result.rightleft	-=	1000.0f;}	//��
	if((diks[0xcd] & 0x80)||(diks[0x4d] & 0x80 )){ result.btn |= KEY_RIGHT;	result.rightleft	+=	1000.0f;}	//�E
	if((diks[0xc8] & 0x80)||(diks[0x48] & 0x80 )){ result.btn |= KEY_UP;	result.updown		-=	1000.0f;}	//��
	if((diks[0xd0] & 0x80)||(diks[0x50] & 0x80 )){ result.btn |= KEY_DOWN;	result.updown		+=	1000.0f;}	//��
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
//	TCHAR		strText[512];	// �f�B�o�C�X�̏�Ԃ��擾����o�b�t�@�i���Ƀ{�^���j
	DIJOYSTATE2	pJoyStickState;	// �W���C�X�e�B�b�N�̏�Ԃ��擾����
//	TCHAR*		str;

	if( NULL == pJoyStick ) 
	{ 
		return ( S_OK );
	}

	//�W���C�X�e�B�b�N�̐ڑ��{�����ԍ���菭�Ȃ����NG
	if( JoyStickNum >= JoyStickCount ) 
	{ 
		return ( S_OK );
	}
	// �f�B�o�C�X�̌��݂̏󋵂��擾����
	hr = pJoyStick[JoyStickNum]->Poll(); 
	if( FAILED(hr) )  
	{
		// DirectInput��Poll���\�b�h���Ăяo���ꂽ���_�ł̓��̓X�g���[���̏�Ԃ�`���邪
		// ���̓f�B�o�C�X�����炩�̌����Ŏ���ꂽ�Ƃ��Ă���������\�b�h�Ԃ��G���[�ȊO�Œm�邱�Ƃ͏o���Ȃ��B
		// �܂��A�f�B�o�C�X�����Z�b�g�����i�������B
		// ���̂��߁A�f�B�o�C�X���Ď擾���邱�Ƃ����݂�B
//		if(  hr == DIERR_NOTACQUIRED){return S_OK;}
		hr = pJoyStick[JoyStickNum]->Acquire();
		while( hr == DIERR_INPUTLOST ) { hr = pJoyStick[JoyStickNum]->Acquire(); }

		// ����ȊO�̃G���[�́A�ق��̃A�v���P�[�V�������t�H�A�O�����h�ɂȂ��Ă��邩
		// �A�v���P�[�V�������̂��ŏ�������ăf�[�^���擾�ł��Ȃ��ꍇ���قƂ�ǁB
		// ���������ꍇ�́A���������ɋA���Č�ōĎ擾�����݂�
		return ( S_OK ); 
	}

	// �W���C�X�e�B�b�N�̏�Ԃ��擾����
	// �����Ŏ擾�ł����Ԃ́A���Poll()���\�b�h���Ăяo�������_�̂���
	// GetDeviceState�̌Ăяo�����x���΁A���R���ꂾ���Â����ɂȂ��Ă��܂�
	// �������G���[��Ԃ��ꍇ�A�W���C�X�e�B�b�N�����������ꂽ�\��������
	if( FAILED( hr = pJoyStick[JoyStickNum]->GetDeviceState( sizeof(DIJOYSTATE2), &pJoyStickState ) ) ){ return ( hr );}

    // ����̃T���v���ŕK�v�Ȃ̂́A����������{�^���Ƃ������̃{�^�������Ȃ̂ŁA����ȊO�̏��͖�������

    // �����{�^���̌��o�iZ���͌��Ă��Ȃ��j
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


   
	// �ǂ̃{�^���������ꂽ���m�F����
	// rgbButtons�͉����ꂽ�{�^���ɑΉ������z��̍ŏ�ʃr�b�g��1�ɂȂ�
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

	//���͂���Ă��Ȃ��L�[���`�F�b�N����
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

	//�����̕ۑ�
	for (int i=INPUT_COUNT-1; i>0; i--) InputHistory[i]=InputHistory[i-1];
	InputHistory[0]=result->btn;

	//�R�}���h�`�F�b�N
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
	//�R�}���h�̍Ō�𒲂ׂ�
	int i=0, j=0;
	while (!(command[i]==KEY_ERR)) i++;

	//�R�}���h�̓��͎��Ԃ��擾����
	int time=command[0];

	//���͗����𒲂ׂāA
	//�R�}���h�����������͂���Ă����True
	for (i--; i>=1; i--) {
		int input=command[i];//���R�}���h���`�F�b�N����
		while (j<INPUT_COUNT && (InputHistory[j]&input)!=input){j++;}//�����ƃR�}���h���r���A�Ⴄ�ԃ��[�v
		if (j>=time || j==INPUT_COUNT) 
			return false;//�������Ԃ��I�[�o�[�������̖����֗���
	}
	return true;
}

void Input::CleanUpDirectInput( void )
{
	// DirectInput�ŃL�[�{�[�h�A�W���C�X�e�B�b�N�A�}�E�X�̂����ꂩ���g�p���������ꍇ�A�g�p���I������B
	if( pKeyboard ){ pKeyboard->Unacquire(); }    
	SAFE_RELEASE( pKeyboard );

	for(size_t i = 0;i < JoyStickCount;++i){
		if( pJoyStick ){ pJoyStick[i]->Unacquire(); }
		SAFE_RELEASE( pJoyStick[i] );
	}

	if( pMouse ){ pMouse->Unacquire(); }
	SAFE_RELEASE( pMouse );

	// DirectInput�̉���B
	SAFE_RELEASE( pDirectInput );
}