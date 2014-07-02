#define STRICT
#define D3D_DEBUG_INFO
//#define WIN32_LEAN_AND_MEAN

#include <string>
#include <d3dx9.h>

//メモリリークチェック用
#if defined _DEBUG
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "Common.h"
#include "Window.h"
#include "Error.h"


int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//生成
	App* app = NULL;

	try{
		app = new App(hInstance,FALSE);
	}catch(char *str){
		DisplayErrMsg(str);
		SAFE_DELETE(app);
		return(0);
	}

	//実行
	app->Run();

	//消去
	SAFE_DELETE(app);

	return(0);
}