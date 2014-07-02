#ifndef __MESH_PLAYER_H__
#define __MESH_PLAYER_H__

#include "Player.h"
#include "MeshObject.h"

class MeshPlayer:public Player{
public:
	MeshObject* obj;
	MoveMode	mode;

	//ç∑ï™Åià⁄ìÆó Åj
	D3DXVECTOR3 difference;

	MeshPlayer(Camera* cam,MeshObject* obj );
	~MeshPlayer();

	void MoveAbsolutely(KeyMessage* keyMessage);
	void MoveRelatively(KeyMessage* keyMessage);
	void FlyInTheSky(KeyMessage* keyMessage);
	void Gravity();
	void Move(KeyMessage* keyMessage);
};

#endif	/*__MESH_PLAYER_H__*/