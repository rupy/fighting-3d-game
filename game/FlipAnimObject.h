#ifndef __FLIP_ANIM_OBJECT_H__
#define __FLIP_ANIM_OBJECT_H__

#include "FlipAnim.h"
#include "MySpriteObject.h"

class FlipAnimObject:public MySpriteObject{
public:
	FlipAnimController FAController;
	FlipAnimObject(int width,int height,
		bool loop = true,int speed = 5,
		CenterPositionFlag center = CPF_LEFT_BOTTOM,
		D3DXVECTOR3 position=D3DXVECTOR3(0,0,0)
		);

	~FlipAnimObject();

	void Move();
	
	HRESULT SetupFlipAnim(char* filename);
	HRESULT SetupFlipAnim(char* filename,int width,int height);
};
#endif	/*__FLIP_ANIM_OBJECT_H__*/