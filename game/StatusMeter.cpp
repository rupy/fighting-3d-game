#include "StatusMeter.h"

StatusMeter::StatusMeter(D3DXVECTOR3 pos)
:back(180,16,CPF_LEFT_BOTTOM,0,pos),left(180,16,CPF_LEFT_BOTTOM,0,pos),
value(0),valuemax(1)
{
	back.SetupTexture(METER_BACK_FILE,180,16);
	left.SetupTexture(METER_LEFT_FILE,180,16);

	maxscale = left.Scale.x;
}
StatusMeter::~StatusMeter(){
}
void StatusMeter::SetMeter(int value,int valuemax){
	this->value = value>=0?value:0;
	this->valuemax = valuemax>0?valuemax:1;
	left.Scale.x = maxscale * (float)this->value/this->valuemax;
}
void StatusMeter::Appear(){

	back.Appear();
	left.Appear();
}