#ifndef __TERRIAN_OBJECT_H__
#define __TERRIAN_OBJECT_H__

#include "MeshObject.h"


class TerrianObject :public MeshObject{
public:
	TerrianObject(vector<Effect*>& eff,
		Mesh* type = NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		float Y=0.0f,float R=0.0f,float P=0.0f);
	~TerrianObject();


};


#endif	/*__TERRIAN_OBJECT_H__*/