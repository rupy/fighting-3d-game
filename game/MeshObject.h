#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__

#include "MeshObjectBase.h"
#include "Mesh.h"

#define WALL_ANGLE 0.9f
#define WALL_SLIP 0.1f

#define MAX_OF_NEXT_HEIGHT	1.0f//登れる最大の高さ
#define GRAVITY_WEIGHT		0.2f//重力による落下速度
#define SLIDE_HEAVIY		0.1f//滑る速さ
#define SLIDE_MIN_ANGLE		0.6f//滑る最小

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

	//自分を描画
	void Appear();
	void Shadow();
	HRESULT LoadMeshFromXFile(const char* filename, const char *aTexturePath = DEFAULT_TEXTURE_PATH);


};

#endif	/*__MESHOBJECT_H__*/