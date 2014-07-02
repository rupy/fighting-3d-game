#ifndef __MESH_BASE_H__
#define __MESH_BASE_H__

class MeshBase{
public:
	virtual void GetBoundingSphere(D3DXVECTOR3* center,float* radius)=0;
	virtual void GetBoundingBox(D3DXVECTOR3* min,D3DXVECTOR3* max)=0;
};

#endif	/*__MESH_BASE_H__*/