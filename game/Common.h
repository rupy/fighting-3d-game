#ifndef __COMMON_H__
#define __COMMON_H__

#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include <tchar.h>


#define WINDOW_TITLE "�Q�[��"

//�E�C���h�E�֘A
#define WINDOW_TOP 50
#define WINDOW_LEFT 50
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BASE_WIDTH 800
#define BASE_HEIGHT 600

#define PI_HREF D3DX_PI 

//SPRINTF�p������
#define	STR_LENGTH 512

//�f�o�b�O�p
#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        sprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // �����
#endif

//-----------------------------------------------------------------------------
//�@�}�N���̒�`
//-----------------------------------------------------------------------------
#define unless(expr) if(!(expr))

//���
#define SAFE_DELETE(me)			{ if(me){ delete (me);		( me )=NULL; } }
#define SAFE_DELETE_ARRAY(me)	{ if(me){ delete[] (me);	( me )=NULL; } }
#define SAFE_RELEASE(me)		{ if(me){ (me)->Release();	( me )=NULL; } }

//�����_��
#define RAND_0_1() ((float)rand()/((float)RAND_MAX+1))
#define RAND_05() ((RAND_0_1())-0.5f)

//���΍��W�ϊ�
#define REL_W(abs) ((float)(abs)/(BASE_WIDTH)*WINDOW_WIDTH)
#define REL_H(abs) ((float)(abs)/(BASE_HEIGHT)*WINDOW_HEIGHT)
#define IREL_W(abs) (int((float)(abs)/(BASE_WIDTH)*WINDOW_WIDTH))
#define IREL_H(abs) (int((float)(abs)/(BASE_HEIGHT)*WINDOW_HEIGHT))


#endif	/*__COMMON_H__*/