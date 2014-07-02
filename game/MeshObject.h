#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__

#include "MeshObjectBase.h"
#include "Mesh.h"

#define WALL_ANGLE 0.9f
#define WALL_SLIP 0.1f

#define MAX_OF_NEXT_HEIGHT	1.0f//ìoÇÍÇÈç≈ëÂÇÃçÇÇ≥
#define GRAVITY_WEIGHT		0.2f//èdóÕÇ…ÇÊÇÈóéâ∫ë¨ìx
#define SLIDE_HEAVIY		0.1f//ääÇÈë¨Ç≥
#define SLIDE_MIN_ANGLE		0.6f//ääÇÈç≈è¨

class MeshObject :public MeshObjectBase
{
protected:

	Mesh*					MeshType;


public:


	MeshObject(vector<Effect*>& eff,
		Mesh* type = NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		float Y=0.0f,float R=0.0f,float P=0.0f);
	~MeshObject();
	
	void PushOutOfTerritory(Object* obj);
	bool WallHit(Object* obj,float minDist,float stayHeight=0.0f);
	Mesh* GetMesh();
	bool LandHit(Object* obj,bool slope=false,float stayHeight = 0.0f);
	bool Landing(Object* obj,float stayHeight = 0.0f);

	//é©ï™Çï`âÊ
	void Appear();
	void Shadow();
	HRESULT LoadMeshFromXFile(const char* filename, const char *aTexturePath = DEFAULT_TEXTURE_PATH);


};

#endif	/*__MESHOBJECT_H__*/