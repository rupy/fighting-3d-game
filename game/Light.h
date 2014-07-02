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

	//void setLightDirection(D3DXVECTOR3 direction);//����
	//void setLightColor(D3DCOLORVALUE color);//�F
	void LightSetting();//���C�g���Z�b�g
	//void LightSwitchOnOff(BOOL on_off);//�n�m�܂��͂n�e�e��ݒ�
	//void LightChangeOnOff();//���C�g�X�C�b�`�̐؂�ւ�Recet
	//void LightRecet();
};

#endif	/*__LIGHT_H__*/