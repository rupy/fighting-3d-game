#include "FieldMap.h"

FieldMap::FieldMap()
:pMesh(NULL)
{
	for (int i=0; i<3; i++)
		texture_name[i] = 0;

	SetMaterial(&Material[0], 1.0f, 1.0f, 1.0f);
	SetMaterial(&Material[1], 0.0f, 0.0f, 0.0f);
	SetMaterial(&Material[2], 1.0f, 1.0f, 1.0f);
}
FieldMap::~FieldMap(){
}

void FieldMap::SetVertexData(
	D3DVERTEX* pV,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float x4, float y4, float z4,
	float nx, float ny, float nz,
	float u1, float v1, float u2, float v2)
{
	pV[0].x = x1;
	pV[0].y = y1;
	pV[0].z = z1;
	pV[0].nx = nx;
	pV[0].ny = ny;
	pV[0].nz = nz;
	pV[0].tu0 = u1;
	pV[0].tv0 = v1;

	pV[1].x = x2;
	pV[1].y = y2;
	pV[1].z = z2;
	pV[1].nx = nx;
	pV[1].ny = ny;
	pV[1].nz = nz;
	pV[1].tu0 = u1;
	pV[1].tv0 = v2;

	pV[2].x = x3;
	pV[2].y = y3;
	pV[2].z = z3;
	pV[2].nx = nx;
	pV[2].ny = ny;
	pV[2].nz = nz;
	pV[2].tu0 = u2;
	pV[2].tv0 = v2;

	pV[3].x = x4;
	pV[3].y = y4;
	pV[3].z = z4;
	pV[3].nx = nx;
	pV[3].ny = ny;
	pV[3].nz = nz;
	pV[3].tu0 = u2;
	pV[3].tv0 = v1;
}


void SetMaterial(D3DMATERIAL9 *material, float r, float g, float b)
{
	ZeroMemory(material, sizeof(D3DMATERIAL9));
	material->Diffuse.r = r;
	material->Diffuse.g = g;
	material->Diffuse.b = b;
	material->Ambient.r = r;
	material->Ambient.g = g;
	material->Ambient.b = b;
	material->Specular.r = 1.0f;
	material->Specular.g = 1.0f;
	material->Specular.b = 1.0f;
	material->Emissive.r = 0;
	material->Emissive.g = 0;
	material->Emissive.b = 0;
	material->Power = 10;
}