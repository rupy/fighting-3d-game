#include "AttachObject.h"

AttachObject::AttachObject(vector<Effect*>& eff,
						   Mesh* type)
:MeshObject(eff,type),rot(true)
{
	validParent = true;
}
D3DXMATRIXA16 AttachObject::MatrixSetting(){

	D3DXMATRIXA16 matPos;		//�ʒu
	D3DXMATRIXA16 matRot;		//��]
	D3DXMATRIXA16 matScale;		//�X�P�[��
	D3DXMATRIXA16 matResult;	//����
	D3DXMATRIXA16 matParent;	//�e�̍s��

	D3DXMatrixIdentity(&matPos);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matResult);
	D3DXMatrixIdentity(&matParent);

	//�L���Ȃ�e���v�Z����
	if(validParent){
		if(rot){
			//��]�܂�
			matParent = parent;
		}else{
			//�A�^�b�`�̂�
			matParent._41 = parent._41;
			matParent._42 = parent._42;
			matParent._43 = parent._43;
		}
	}

	D3DXMatrixTranslation(&matPos, Pos.x, Pos.y, Pos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, Yaw, Roll, Pitch);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matResult = matScale * matRot * matPos * matParent;

	return matResult;

}
void AttachObject::SetParentMatrix(D3DXMATRIX parentmat){
	parent = parentmat;
}