#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <d3dx9.h>
#include "Effect.h"
#include "ResourceBase.h"

class Light:public ResourceBase
{
	//D3DLIGHT9 light;
	vector<Effect*>& rEff;
public:
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 lookatPt;
	D3DXVECTOR4 dir;
public:
	Light(vector<Effect*>& eff, D3DXVECTOR3 pos);
	~Light();

	//void setLightDirection(D3DXVECTOR3 direction);//方向
	//void setLightColor(D3DCOLORVALUE color);//色
	void LightSetting();//ライトをセット
	//void LightSwitchOnOff(BOOL on_off);//ＯＮまたはＯＦＦを設定
	//void LightChangeOnOff();//ライトスイッチの切り替えRecet
	//void LightRecet();
};

#endif	/*__LIGHT_H__*/