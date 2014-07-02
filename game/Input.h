#ifndef __INPUT_H__
#define __INPUT_H__

#define DIRECTINPUT_VERSION 0x0800

#include <d3dx9.h>
#include <dinput.h>

class KeyMessage
{
public:
	float			updown;	        // スティックの上下
	float			rightleft;		// スティックの右左
	float			zupdown;        // スティックの上下
	float			zrightleft;		// スティックの右左
	unsigned long	btn;            // ボタン情報
	unsigned long	cmd;			// 新規ボタン情報

	KeyMessage();
	~KeyMessage();	
};

//ヒストリに保存する数
#define INPUT_COUNT			10

//ジョイスティックの数
#define INPUT_DEVICE_NUM	4

class Input{
public:
	HWND					hWnd;

	LPDIRECTINPUT8			pDirectInput;
	LPDIRECTINPUTDEVICE8	pKeyboard;						// キーボードディバイス
	LPDIRECTINPUTDEVICE8	pJoyStick[INPUT_DEVICE_NUM];	// ジョイスティックディバイス
	LPDIRECTINPUTDEVICE8	pMouse;							// マウスディバイス

	size_t					JoyStickCount;

	unsigned long			InputHistory[INPUT_COUNT];

	Input();
	~Input();
	HRESULT InitInputDevice( HWND hWnd, HINSTANCE hInstance );

private:
	HRESULT SetupKeybord();

	HRESULT SetupJoyStick();
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, void* pContext );
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,void* pContext );
	HRESULT InitKeyboard(KeyMessage *result);


	HRESULT SetupMouse();
	void CleanUpDirectInput();
public:
	KeyMessage GetKeybordState();
	HRESULT GetJoystickState(KeyMessage *result, size_t JoyStickNum );
	HRESULT GetCommand(KeyMessage* result);
	bool CommandCheck(const unsigned long *command,KeyMessage* result);
};

enum KeyState{
	//入力あり
	KEY_NONE		= 0x0000000,

	KEY_LEFT		= 0x0000001,
	KEY_RIGHT		= 0x0000002,
	KEY_UP			= 0x0000004,
	KEY_DOWN		= 0x0000008,

	KEY_A			= 0x0000010,// A
	KEY_B			= 0x0000020,// B
	KEY_X			= 0x0000040,// C
	KEY_Y			= 0x0000080,// X
	KEY_S			= 0x0000100,// Y
	KEY_D			= 0x0000200,// Z
	KEY_L			= 0x0000400,// L
	KEY_R			= 0x0000800,// R
	KEY_SELECT		= 0x0001000,
	KEY_START		= 0x0002000,

	//入力なし
	KEY_LEFTn		= 0x0004000,
	KEY_RIGHTn		= 0x0008000,
	KEY_UPn			= 0x0010000,
	KEY_DOWNn		= 0x0020000,

	KEY_An			= 0x0040000,// A
	KEY_Bn			= 0x0080000,// B
	KEY_Xn			= 0x0100000,// C
	KEY_Yn			= 0x0200000,// X
	KEY_Sn			= 0x0400000,// Y
	KEY_Dn			= 0x0800000,// Z
	KEY_Ln			= 0x1000000,// L
	KEY_Rn			= 0x2000000,// R
	KEY_SELECTn		= 0x4000000,
	KEY_STARTn		= 0x8000000,


	KEY_DIR_MASK	= 0x000000f,
	KEY_BUTTON_MASK	= 0xffffff0,
	KEY_ERR			= 0xfffffff
};

enum CommandState{
	//入力あり
	NEW_NONE		= 0x0000000,

	NEW_LEFT		= 0x0000001,
	NEW_RIGHT		= 0x0000002,
	NEW_UP			= 0x0000004,
	NEW_DOWN		= 0x0000008,

	NEW_A			= 0x0000010,// A
	NEW_B			= 0x0000020,// B
	NEW_X			= 0x0000040,// C
	NEW_Y			= 0x0000080,// X
	NEW_S			= 0x0000100,// Y
	NEW_D			= 0x0000200,// Z
	NEW_L			= 0x0000400,// L
	NEW_R			= 0x0000800,// R
	NEW_SELECT		= 0x0001000,
	NEW_START		= 0x0002000,

	CMD_1			= 0x0004000,
	CMD_2			= 0x0008000,
	CMD_3			= 0x0010000,
	CMD_4			= 0x0020000,
	CMD_5			= 0x0040000,// A
	CMD_6			= 0x0080000,// B
	CMD_7			= 0x0100000,// C
	CMD_8			= 0x0200000,// X
	CMD_9			= 0x0400000,// Y
	CMD_10			= 0x0800000,// Z
	CMD_11			= 0x1000000,// L
	CMD_12			= 0x2000000,// R
	CMD_13			= 0x4000000,
	CMD_14			= 0x8000000,

	NEW_DIR_MASK	= 0x000000f,
	NEW_BUTTON_MASK	= 0xffffff0,
	NEW_ERR			= 0xfffffff
};



//　ジョイスティックで通常Aボタン、もしくはCボタンがOK、Bがキャンセルの意味を持つため
#define KEY_YES	(KEY_A | KEY_START | KEY_SELECT | KEY_X)
#define	KEY_NO		(KEY_B)

//　ジョイスティックの精度と感度
#define	JS_RESO	1000
#define	JS_DEAD	( JS_RESO / 4 )



static const unsigned long
//=======================================================
//コマンド		time	0			1			2			
//=======================================================
CMD_LEFT[]		= {2,	KEY_LEFTn,	KEY_LEFT,	KEY_ERR},
CMD_RIGHT[]		= {2,	KEY_RIGHTn,	KEY_RIGHT,	KEY_ERR},
CMD_UP[]		= {2,	KEY_UPn,	KEY_UP,		KEY_ERR},
CMD_DOWN[]		= {2,	KEY_DOWNn,	KEY_DOWN,	KEY_ERR},
CMD_A[]			= {2,	KEY_An,		KEY_A,		KEY_ERR},
CMD_B[]			= {2,	KEY_Bn,		KEY_B,		KEY_ERR},
CMD_X[]			= {2,	KEY_Xn,		KEY_X,		KEY_ERR},
CMD_Y[]			= {2,	KEY_Yn,		KEY_Y,		KEY_ERR},
CMD_S[]			= {2,	KEY_Sn,		KEY_S,		KEY_ERR},
CMD_D[]			= {2,	KEY_Dn,		KEY_D,		KEY_ERR},
CMD_L[]			= {2,	KEY_Ln,		KEY_L,		KEY_ERR},
CMD_R[]			= {2,	KEY_Rn,		KEY_R,		KEY_ERR},
CMD_SELECT[]	= {2,	KEY_SELECTn,KEY_SELECT,	KEY_ERR},
CMD_START[]		= {2,	KEY_STARTn,	KEY_START,	KEY_ERR};
#endif	/*__INPUT_H__*/