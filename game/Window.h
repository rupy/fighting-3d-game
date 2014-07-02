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
	HANDLE		hMutex;			//�~���[�e�b�N�X
	WNDCLASSEX	wc;				//�E�C���h�E�N���X
	HINSTANCE	hInstance;		//�C���X�^���X�n���h��
	HWND		hWnd;			//�E�C���h�n���h��
	bool		isFullScreen;	//�t���X�N���[��ON/OFF
	bool		lostFocus;		//�E�C���h�E�Ƀt�H�[�J�X�����邩�ǂ���
	bool		isPause;		//�Q�[���̒��f

	Renderer	renderer;		//Direct3D�f�o�C�X�Ȃǂ̐����A�ێ��N���X
	GameBase*	game;			//�Q�[���N���X�i�Q�[���̖{�́j
	Input		input;			//DirectInput�f�o�C�X�̐����A���͊Ǘ�
	Sound		sound;			//DirectSound�f�o�C�X�̐����A�T�E���h�Ǘ�


public:


	//-------------------------------------------------------------
	// �R���X�g���N�^�i�A�v���P�[�V�����̐����j
	// ����
	//		hInstance	: WinMain�֐�����󂯎�����C���X�^���X�n���h��
	//		isFullScreen: �t���X�N���[��ON/OFF
	//-------------------------------------------------------------
	App(HINSTANCE hInstance, bool isFullScreen);
	//-------------------------------------------------------------
	// �f�X�g���N�^�i�~���[�e�b�N�X�̊J���Ȃǁj
	// ����
	//		hInstance	: WinMain�֐�����󂯎�����C���X�^���X�n���h��
	//		isFullScreen: �t���X�N���[��ON/OFF
	//-------------------------------------------------------------
	~App();

	//�A�v���P�[�V�����̎��s
	HRESULT Run();

private:
	//�E�C���h�E�N���X�̓o�^
	HRESULT RegistWndClass();
	//�E�C���h�E�̐���
	HRESULT CreateMainWnd();
	//���j���[�o�[�̐���
	void CreateMenuBar();
	//���b�Z�[�W���[�v
	HRESULT MsgLoop();

	//�E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK DummyProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//�_�~�[
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�t���X�N���[���؂�ւ�
	HRESULT ToggleFullScreen(bool isFullScreen);
};

//���j���[�o�[�I���̃��b�Z�[�W
enum MenuBarMsgEnum{
	IDM_NEW = 40001,//�V�K�쐬
	IDM_SETTING,	//�ݒ�
	IDM_CLOSE,		//�I��
	IDM_FULLSCREEN,	//�t���X�N���[���؂�ւ�
	IDM_VERSION		//�o�[�W�������
};

#endif	/*__WINDOW_H__*/