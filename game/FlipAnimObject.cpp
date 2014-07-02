#include "FlipAnimObject.h"

FlipAnimObject::FlipAnimObject(int width, int height,
							   bool loop,int speed,
							   CenterPositionFlag center,
							   D3DXVECTOR3 position)
							   :MySpriteObject(width,height,center,0,position),
							   FAController(loop,speed)
{
}

void FlipAnimObject::Move(){
	FAController.NextTime();
	pTex = FAController.GetCurrectTexture();
}

HRESULT FlipAnimObject::SetupFlipAnim(char *filename){
	if(FAILED(FAController.SetupTexture(filename))){
		return E_FAIL;
	}
	pTex = FAController.GetCurrectTexture();
	return S_OK;
}
HRESULT FlipAnimObject::SetupFlipAnim(char* filename,int width,int height){
	if(FAILED(FAController.SetupTexture(filename,width,height))){
		return E_FAIL;
	}
	pTex = FAController.GetCurrectTexture();
	return S_OK;
}