#ifndef __MODELS_H__
#define __MODELS_H__

#include <windows.h>
#include <d3dx9.h>
#include "MeshBase.h"

#define DEFAULT_TEXTURE_PATH "models/"

class Mesh :public MeshBase
{
public:

	LPD3DXMESH				objMesh;			// システム中のメッシュオブジェクト
	D3DMATERIAL9*			objMeshMaterials;	// メッシュの質感(materials)
	LPDIRECT3DTEXTURE9*		objMeshTextures;	// メッシュのテクスチャ(texture)
	DWORD					objNumOfMaterials;	// メッシュ質感の数
private:
	//当たり判定
	D3DXVECTOR3				objCenter;
	float					objRadius;
	D3DXVECTOR3				objMin;
	D3DXVECTOR3				objMax;

public:
	Mesh();
	~Mesh();
	HRESULT LoadModelsFromXFile( LPDIRECT3DDEVICE9 theDevice, const char *aString , const char *aTexturePath = DEFAULT_TEXTURE_PATH);
	HRESULT CalcBoundingSphere();
	HRESULT CalcBoundingBox();
	void	GetBoundingSphere(D3DXVECTOR3* center,float* radius);
	void	GetBoundingBox(D3DXVECTOR3* min,D3DXVECTOR3* max);
	HRESULT FindVerticesOnPoly(DWORD index, D3DXVECTOR3* vertices);
};



#endif	/*__MODELS_H__*/