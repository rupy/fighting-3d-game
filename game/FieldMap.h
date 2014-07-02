#ifndef __FIELDMAP_H__
#define __FIELDMAP_H__

#include <d3dx9.h>


class FieldMap{
public:
	struct D3DVERTEX {
		float x, y, z;
		float nx, ny, nz;
		float tu0, tv0;

//		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 } ;
	} ;

	LPD3DXMESH	pMesh;
	D3DMATERIAL9 Material[3];
//	CTexture	Texture[3];
	char *texture_name[3];


	FieldMap();
	~FieldMap();
	
	void SetVertexData(D3DVERTEX* pV,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float x4, float y4, float z4,
	float nx, float ny, float nz,
	float u1, float v1, float u2, float v2);
};

void SetMaterial(D3DMATERIAL9 *material, float r, float g, float b);

#endif	/*__FIELDMAP_H__*/