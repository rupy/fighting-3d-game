#ifndef __DUMMY_OBJECT_H__
#define __DUMMY_OBJECT_H__

#include "Object.h"

//実体のないオブジェクトなど
class DummyObject:public Object
{

public:

	DummyObject(D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0),
		float Y=0,float R=0,float P=0);

	~DummyObject();

	//マトリックスの計算
	D3DXMATRIXA16 MatrixSetting();
	//自分を描画
	void Appear();

};

#endif	/*__DUMMY_OBJECT_H__*/