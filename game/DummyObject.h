#ifndef __DUMMY_OBJECT_H__
#define __DUMMY_OBJECT_H__

#include "Object.h"

//���̂̂Ȃ��I�u�W�F�N�g�Ȃ�
class DummyObject:public Object
{

public:

	DummyObject(D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0),
		float Y=0,float R=0,float P=0);

	~DummyObject();

	//�}�g���b�N�X�̌v�Z
	D3DXMATRIXA16 MatrixSetting();
	//������`��
	void Appear();

};

#endif	/*__DUMMY_OBJECT_H__*/