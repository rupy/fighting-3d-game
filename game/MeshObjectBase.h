#ifndef __MESH_OBJECT_BASE_H__
#define __MESH_OBJECT_BASE_H__

#include "Object.h"
#include <vector>
#include "Effect.h"
#include "MeshBase.h"

using namespace std;
class MeshObjectBase:public Object{
protected:
	bool					myMesh;
	vector<Effect*>& rEff;
public:
	MeshObjectBase(vector<Effect*>& eff,D3DXVECTOR3 pos, float Y,float R,float P)
		:Object(pos,Y,R,P),rEff(eff),myMesh(false)
	{}
	
	virtual MeshBase* GetMesh()=0;
};

#endif	/*__MESH_OBJECT_BASE_H__*/