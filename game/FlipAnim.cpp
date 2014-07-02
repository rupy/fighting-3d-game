#include "FlipAnim.h"
#include "Common.h"

FlipAnimController::FlipAnimController(bool loop,int speed)
:time(0),page(0),speed(speed),loop(loop),startpage(0),endpage(0)
{
}
FlipAnimController::~FlipAnimController(){

	int pagecount = (int)textures.size();
	for(int i=0;i < pagecount;i++){
		SAFE_RELEASE(textures[i]);
	}
}

bool FlipAnimController::NextTime(){

	time++;
	if(time >= speed){
		time = 0;
		if(page+1 >= (int)textures.size()){
			//終わりに達した
			if(loop){
				page = 0;
			}else{
				return false;
			}
		}
		page++;
	}
	return true;
}
void FlipAnimController::SetSpeed(int speed){
	this->speed = speed;
}
LPDIRECT3DTEXTURE9 FlipAnimController::GetCurrectTexture(){
	return textures[page];
}

HRESULT FlipAnimController::SetupTexture(char* tex_file_name){

	LPDIRECT3DTEXTURE9 tex;
	if(FAILED(D3DXCreateTextureFromFile(GetDevice(), tex_file_name ,&tex))){
		DisplayErrMsg("テクスチャの読み込みに失敗しました");	
		return(E_FAIL);
	}
	textures.push_back(tex);
	endpage = textures.size();
	return S_OK;
}
HRESULT FlipAnimController::SetupTexture(char* tex_file_name,int width,int height){

	LPDIRECT3DTEXTURE9 tex;
	if(FAILED(D3DXCreateTextureFromFileEx( GetDevice(),tex_file_name,width,height,
		0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&tex))){
		DisplayErrMsg("テクスチャの読み込みに失敗しました");	
		return(E_FAIL);
	}
	textures.push_back(tex);
	endpage = textures.size();
	return S_OK;
}