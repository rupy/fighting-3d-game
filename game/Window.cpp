#include "Window.h"
#include "Common.h"
//=============================================================
// �R���X�g���N�^�i�A�v���P�[�V�����̐����j
//=============================================================
App::App(HINSTANCE hInstance, bool isFullScreen)
:game(NULL),
hInstance(hInstance),
isFullScreen(isFullScreen),
lostFocus(FALSE),
isPause(FALSE)
{
	// ���d�N���̃`�F�b�N
	hMutex = CreateMutex(NULL, FALSE, WINDOW_TITLE);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(hMutex){CloseHandle(hMutex);}
		throw "���̃A�v���P�[�V�����͑��d�N���ł��܂���B";
	}

	//�E�C���h�E�N���X�̓o�^
	if(FAILED(RegistWndClass())){
		throw "�E�C���h�E�N���X�̓o�^�Ɏ��s���܂���";
	}

	//�E�C���h�E�̐���
	if(FAILED(CreateMainWnd())){
		throw "�E�C���h�E�N���X�̓o�^�Ɏ��s���܂���";
	}

	//���j���[�o�[�̐���
	if(!isFullScreen){
		CreateMenuBar();
	}

	//DIRECTINPUT�̏�����
	//(Direct3D�������̑O�ɂ���Ă����Ȃ���΃G���[�_�C�A���O������Ȃ��Ȃ�)
	if(FAILED(input.InitInputDevice(hWnd,hInstance))){
		throw "DirectInput�̏������Ɏ��s���܂���";
	}


	//DIRECT3D�̏�����
	if(FAILED(renderer.Initialize(hWnd,isFullScreen,WINDOW_WIDTH,WINDOW_HEIGHT))){
		throw "DirectX�̏������Ɏ��s���܂���";
	}

	//DIRECTSOUND�̏�����
	if(FAILED(sound.InitDirectSound(hWnd))){
		throw "DirectSound�̏������Ɏ��s���܂���";
	}

	game = new GameBase(
		renderer.GetDevice(),
		renderer.GetPParam(),
		renderer.GetSprite(),
		&input,
		&sound);

}
//=============================================================
// �f�X�g���N�^
//=============================================================
App::~App(){
	
	SAFE_DELETE(game);
	//�~���[�e�b�N�X�̉��
	if(hMutex){CloseHandle(hMutex);}
}


//=============================================================
// �E�C���h�E�N���X�̓o�^
//=============================================================
HRESULT App::RegistWndClass(){

	wc.cbSize=sizeof(WNDCLASSEX);					// ���̍\���̂̃T�C�Y
	wc.style=NULL;									// �E�C���h�E�̃X�^�C��(default)
	wc.lpfnWndProc=DummyProc;						// �_�~�[�̃E�C���h�E�v���V�[�W���̓o�^
	wc.cbWndExtra=0;								// �ʏ�͎g��Ȃ��̂ŏ��0
	wc.cbClsExtra=0;								// �ʏ�͎g��Ȃ��̂ŏ��0
	wc.hInstance=hInstance;							// �C���X�^���X�ւ̃n���h��
	wc.hIcon=NULL;									// �A�C�R���i�Ȃ��j
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);			// �J�[�\���̌`
	wc.hbrBackground=NULL;							// �w�i�Ȃ�
	wc.lpszMenuName=NULL;							// ���j���[�Ȃ�
	wc.lpszClassName=WINDOW_TITLE;					// �N���X���̎w��
	wc.hIconSm=NULL;								// ���A�C�R���i�Ȃ��j

	if( !RegisterClassEx( &wc ) ){
		return(E_FAIL);
	}
	
	return(S_OK);
}
//=============================================================
// �E�C���h�E�̐���
//=============================================================
HRESULT App::CreateMainWnd(){

	//�ő剻�{�^���������Ȃ��E���E�ύX�̂ł��Ȃ�window
	DWORD	dwWindowStyle= ~(WS_MAXIMIZEBOX | WS_THICKFRAME) & WS_OVERLAPPEDWINDOW;

	hWnd = CreateWindow(	WINDOW_TITLE, WINDOW_TITLE, dwWindowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL );

	//�ÓI�����o�֐��̃E�C���h�E�v���V�[�W����this�|�C���^��n�����߁B
	SetWindowLong( hWnd, GWL_USERDATA, (LONG)this );
	//�E�C���h�v���V�[�W���̂���ւ��ithis�|�C���^���ʂ�܂ł͋@�\���Ȃ����߁j
	SetWindowLong( hWnd, GWL_WNDPROC, (LONG)WindowProc);

	// �E�B���h�E�T�C�Y���Đݒ肷��
	RECT rect;
	int ww, wh;
	int cw, ch;

	// �E�C���h�E�S�̂̉����̕����v�Z
	GetWindowRect(hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
	ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
	wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z

	// �N���C�A���g�̈�̊O�̕����v�Z
	GetClientRect(hWnd, &rect);		// �N���C�A���g�����̃T�C�Y�̎擾
	cw = rect.right - rect.left;	// �N���C�A���g�̈�O�̉������v�Z
	ch = rect.bottom - rect.top;	// �N���C�A���g�̈�O�̏c�����v�Z

	ww = ww - cw;					// �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
	wh = wh - ch;					// �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

	// �E�B���h�E�T�C�Y�̍Čv�Z
	ww = WINDOW_WIDTH + ww;			// �K�v�ȃE�C���h�E�̕�
	wh = WINDOW_HEIGHT + wh;		// �K�v�ȃE�C���h�E�̍���

	// �E�C���h�E�T�C�Y�̍Đݒ�
	SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	// �E�B���h�E�̕\��
    ShowWindow(hWnd, SW_SHOWDEFAULT);

	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);


	return(S_OK);
}
//=============================================================
// ���j���[�o�[�̐���
//=============================================================
void App::CreateMenuBar(){
	HMENU hMenu;
	HMENU hSubMenu;
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);

	hMenu = CreateMenu();
	hSubMenu = CreatePopupMenu();
	//���j���[�̒ǉ�
	//�Q�[��(&G)
	//	>�V�K�쐬(&N)
	//	>�ݒ�(&S)
	//	>�I��(&X)
	//�t���X�N���[��(&F)
	//�o�[�W�������(&H)

	mii.dwTypeData = _T("�V�K�쐬(&N)");
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.fType = MFT_STRING;
	mii.wID = IDM_NEW;
	InsertMenuItem(hSubMenu,0,TRUE,&mii);

	mii.dwTypeData = _T("�ݒ�(&S)");
	mii.wID = IDM_SETTING;
	InsertMenuItem(hSubMenu,1,TRUE,&mii);

	mii.dwTypeData = _T("�I��(&X)");
	mii.wID = IDM_CLOSE;
	InsertMenuItem(hSubMenu,2,TRUE,&mii);


	mii.fMask = MIIM_TYPE|MIIM_SUBMENU;
	mii.dwTypeData = _T("�Q�[��(&G)");
	mii.hSubMenu = hSubMenu;
	InsertMenuItem(hMenu,0,TRUE,&mii);

	mii.dwTypeData = _T("�t���X�N���[��(&F)");
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.hSubMenu = NULL;
	mii.wID = IDM_FULLSCREEN;
	InsertMenuItem(hMenu,1,TRUE,&mii);

	mii.dwTypeData = _T("�o�[�W�������(&H)");
	mii.wID = IDM_VERSION;
	InsertMenuItem(hMenu,2,TRUE,&mii);

	SetMenu(hWnd,hMenu);//���j���[��ݒ�
}
//=============================================================
//�@���b�Z�[�W���[�v
//=============================================================
HRESULT App::MsgLoop()
{
	// ���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	DWORD dwCurTime=0;
	DWORD dwLastTime=0;
	BOOL bMove=TRUE;
		

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage (&msg,NULL,0,0,PM_NOREMOVE)) 
		{
			if( !GetMessage (&msg,NULL,0,0) ){ msg.message = WM_QUIT; }	// �I��
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

			//�ړ��ƕ`��
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

			//�\��
			if (dwCurTime - dwLastTime>=FLAME_RATE) {
				if(FAILED(game->Display())){
					MessageBox( NULL, "�\�����Ȃ��f�o�C�X���X�g���N���܂����B", WINDOW_TITLE, MB_OK|MB_ICONHAND );
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
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK App::DummyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK App::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//this�|�C���^���󂯎��A�E�C���h�v���V�[�W�����Ŏg����悤�ɂ���
	App* pThis = (App*)GetWindowLong( hWnd, GWL_USERDATA );

	switch (msg) {
	//�E�C���h�E������ꂽ
	case WM_CLOSE:
		PostQuitMessage(0);			// �A�v���P�[�V�������I������
		break;
	//�L�[�������ꂽ
	case WM_KEYDOWN:
		//ESC�L�[
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);		// �A�v���P�[�V�������I������
		}else
		//SPACE�L�[
		if (wParam == VK_SPACE) {
			pThis->ToggleFullScreen(FALSE);
		}else 
		//F1�L�[
		if (wParam == VK_F1 ) {
			pThis->isPause = !pThis->isPause;
		}
		break;
	//�t�H�[�J�X���߂���
	case WM_SETFOCUS:
		pThis->lostFocus = FALSE;
		break;
	//�t�H�[�J�X��������
	case WM_KILLFOCUS:
		pThis->lostFocus = TRUE;
		break;
	//���j���[�o�[�֘A
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDM_CLOSE:
			PostQuitMessage(0);		// �A�v���P�[�V�������I������
			break;
		case IDM_VERSION:
			MessageBox( NULL, "�o�[�W�����P�ł��B\n", "�o�[�W�������", MB_OK | MB_SETFOREGROUND|MB_TOPMOST );
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
// �A�v���P�[�V�����̎��s
//=============================================================
HRESULT App::Run(){

	if(FAILED(MsgLoop())){
		return(E_FAIL);
	}
	return(S_OK);
}

//=============================================================
// �t���X�N���[���̐؂�ւ�
//=============================================================
HRESULT App::ToggleFullScreen(bool isFullScreen){

  	this->isFullScreen = isFullScreen;

	D3DPRESENT_PARAMETERS* pD3DPP=renderer.GetPParam();
	if(isFullScreen){
		SetMenu(hWnd,NULL);

		pD3DPP->Windowed				= FALSE;			// �t���X�N���[���\���̎w��
		pD3DPP->BackBufferWidth			= WINDOW_WIDTH;		// �t���X�N���[�����̉���
		pD3DPP->BackBufferHeight		= WINDOW_HEIGHT;		// �t���X�N���[�����̏c��
	}else{
		CreateMenuBar();
		pD3DPP->Windowed				= TRUE;			// �t���X�N���[���\���̎w��	
	}

	game->DeviceRecet();

	return S_OK;
}