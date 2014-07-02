#ifndef __ATTACH_OBJECT_H__
#define __ATTACH_OBJECT_H__

#include "MeshObject.h"

class AttachObject:public MeshObject{
public:
	AttachObject(vector<Effect*>& eff,
		Mesh* type);
	D3DXMATRIXA16 parent;
	bool rot;

	D3DXMATRIXA16 MatrixSetting();
	void SetParentMatrix(D3DXMATRIX parentmat);
};

#endif	/*__ATTACH_OBJECT_H__*/