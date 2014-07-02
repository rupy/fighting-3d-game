#include "TerrianObject.h"
#include "Math.h"
#include "VectorTest.h"

TerrianObject::TerrianObject(vector<Effect*>& eff,
							 Mesh* type,
							 D3DXVECTOR3 position,
							 float Y,float R,float P)
							 :MeshObject(eff,type,position,Y,R,P)
{
}
TerrianObject::~TerrianObject(){
}
