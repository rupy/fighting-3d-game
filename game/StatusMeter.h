#ifndef __STATUS_METER_H__
#define __STATUS_METER_H__

#include "MySpriteObject.h"
#define METER_BACK_FILE "picture/MeterBack.png"
#define METER_LEFT_FILE "picture/MeterLeft.png"

class StatusMeter{
public:
	int value;
	int valuemax;
	float maxscale;
	MySpriteObject	back;
	MySpriteObject	left;

	void SetMeter(int value,int valuemax);
	StatusMeter(D3DXVECTOR3 pos);
	~StatusMeter();

	void Appear();
};

#endif	/*__STATUS_METER_H__*/