#ifndef __ERROR_H__
#define __ERROR_H__

#include <windows.h>

void DisplayErrMsg(const char* err_msg);
void DisplayErrMsg(const char* err_msg,const char* title);
void ErrAndQuit(const char* err_msg);

#endif	/*__ERROR_H__*/