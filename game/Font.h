#ifndef __FONT_H__
#define __FONT_H__

#include <d3dx9.h>
class Font
{
public:
	LPDIRECT3DTEXTURE9 pTex;
	float fTexW; // テクスチャの横幅
	float fTexH; // テクスチャの高さ
	//デフォルトコンストラクタ
	Font();
	//デストラクタ
	~Font();

	HRESULT MakeFont(LPDIRECT3DDEVICE9 device,const TCHAR* ch,int fontsize);
};

#endif	/*__FONT_H__*/