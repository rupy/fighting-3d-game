#ifndef __WATER_FIELD_H__
#define __WATER_FIELD_H__

#include "VertexObject.h"

#define SEA_TEXTURE_FILE "picture/water.jpg"
class WaterField:public Object{
public:

	int size;
	float scale;
	int time;
	int speed;
	int wavetime;
	int wavespeed;
	VertexObject water;

	WaterField(vector<Effect*>& eff,
		D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0));
	~WaterField();

	void Move();
	void Appear();
	HRESULT SetupTexture();

};

#endif	/*__WATER_FIELD_H__*/