#ifndef __BILLBOARD_OBJECT_H__
#define __BILLBOARD_OBJECT_H__

#include "VertexObject.h"

class BillboardObject: public VertexObject{
public:
	BillboardObject(vector<Effect*>& eff,
		LPDIRECT3DTEXTURE9 tex=NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0,0,0),
		float Y=0.0f,float R=0.0f,float P=0.0f);

	~BillboardObject();

	void LookAtMe(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	void Appear();

};


#endif	/*__BILLBOARD_OBJECT_H__*/