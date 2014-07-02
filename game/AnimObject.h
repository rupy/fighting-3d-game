#ifndef __ANIMOBJECT_H__
#define __ANIMOBJECT_H__

#include "MeshObjectBase.h"
#include "AnimMesh.h"
#include "MeshObject.h"


class AnimObject :public MeshObjectBase
{
protected:

	AnimMesh*	MeshType;
	int			animIndex;
	bool		animStop;
	float		animSpeed;
	bool		animChange;
	bool		damage;

	ModelAnimation* oldAnim;


public:

	AnimObject(vector<Effect*>& eff,
		AnimMesh* type,
		int animIndex,
		D3DXVECTOR3 position=D3DXVECTOR3(0.0f,0.0f,0.0f),
		float Y=0.0f,float R=0.0f,float P=0.0f);
	~AnimObject();
	


	//Ž©•ª‚ð•`‰æ
	void Shadow();
	void Appear();
	void Animation();
	D3DXMATRIX GetAttachMatrix(char* framename);
	HRESULT LoadMeshFromXFile(char* filename);
	void SetMesh(AnimMesh* mesh,bool give_clone = true);
	int GetAnimIndex();
	bool GetAnimStop();
	float GetAnimSpeed();
	bool GetAnimChange();
	bool GetDamage();
	void SetAnimIndex(int index);
	void SetAnimStop(bool stop);
	void SetAnimSpeed(float speed);
	void SetAnimChange(bool change);
	void SetDamage(bool damage);
	void AnimStop();
	void AnimStart();
	void NoAnimChange();
	void OkAnimChange();
	void NoDamage();
	void OkDamage();
	AnimMesh* GetMesh();

};
#endif	/*__ANIMOBJECT_H__*/