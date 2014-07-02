#include "Error.h"

void DisplayErrMsg(const char* err_msg){
		MessageBox( NULL, err_msg, "エラー", MB_OK|MB_ICONHAND );	
}

void DisplayErrMsg(const char* err_msg,const char* title){
		MessageBox( NULL, err_msg, title, MB_OK|MB_ICONHAND );	
}
void ErrAndQuit(const char* err_msg){
		MessageBox( NULL, err_msg, "エラー", MB_OK|MB_ICONHAND );	
		PostQuitMessage(0);
}