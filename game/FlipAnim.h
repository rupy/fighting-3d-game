#ifndef __FLIP_ANIM_H__
#define __FLIP_ANIM_H__

#include <d3dx9.h>
#include <vector>
#include "Error.h"
#include "ResourceBase.h"

using namespace std;
//パラパラ漫画用テクスチャ切り替えインターフェイス

class FlipAnimController:public ResourceBase{
public:
	int time;
	int speed;
	int page;
	bool loop;
	int startpage;
	int endpage;
	vector<LPDIRECT3DTEXTURE9> textures;

	FlipAnimController(bool loop = true,int speed = 5);
	~FlipAnimController();
	bool NextTime();
	void SetSpeed(int speed);
	LPDIRECT3DTEXTURE9 GetCurrectTexture();
	HRESULT SetupTexture(char* tex_file_name);
	HRESULT SetupTexture(char* tex_file_name,int width,int height);
};
#endif	/*__FLIP_ANIM_H__*/