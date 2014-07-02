#include "Light.h"

Light::Light(vector<Effect*>& eff, D3DXVECTOR3 pos)
:rEff(eff),Pos(pos),lookatPt(D3DXVECTOR3(0,0,0)),dir(D3DXVECTOR4(0,1.0f,0,0))
{
}
Light::~Light(){}

void Light::LightSetting(){

//	D3DXVECTOR4 tmpLightDirection;
//	tmpLightDirection = D3DXVECTOR4(Pos - lookatPt,0.0f);
//	D3DXVec4Normalize(&tmpLightDirection, &tmpLightDirection);
	if(FAILED(rEff[0]->SetVector(&dir,"g_light_dir"))){
		return;
	}
//	MyOutputDebugString("<%f,%f,%f>\n",tmpLightDirection.x,tmpLightDirection.y,tmpLightDirection.z);
	// �r���[�|�[�g���e�N�X�`���摜�̃T�C�Y�ɐݒ�
	D3DVIEWPORT9 vp;
	GetDevice()->GetViewport(&vp);

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	D3DXMATRIXA16	proj,view;
	// �v���W�F�N�V�����}�g���b�N�X�̍쐬�Ɛݒ�
	D3DXMatrixIdentity(&proj);
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45.0f), aspect, 1.0f, 300.0f);

	// ���W�ϊ��i�������J�����ƌ����Ă�j
	D3DXMatrixIdentity(&view);
	D3DXMatrixLookAtLH(&view, 
						&Pos, 
						&lookatPt, 
						&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));


	// �V�F�[�_�[�̃O���[�o���ϐ��̒l�̐ݒ�
	if(FAILED(rEff[1]->SetMatrix(&(view*proj),"g_vlp"))){
		return;
	}
	// ���C�g�̐ݒ�
	if(FAILED(rEff[0]->SetMatrix(&(view*proj),"g_vlp"))){
		return;
	}
}