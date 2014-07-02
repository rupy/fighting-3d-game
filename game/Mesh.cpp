#include "Mesh.h"
#include "Common.h"

Mesh::Mesh()
:objMeshMaterials(NULL),
objMeshTextures(NULL),
objMesh(NULL)
{}
Mesh::~Mesh(){

		/*メッシュマテリアルの解放*/
		SAFE_DELETE_ARRAY(objMeshMaterials);

		/*テクスチャの解放*/
		if(objMeshTextures )
		{
			for(DWORD j0 = 0; j0 < objNumOfMaterials ; j0++ ){ SAFE_RELEASE(objMeshTextures[j0] );}
			SAFE_DELETE_ARRAY(objMeshTextures );
		}
		
		/*メッシュの開放*/
		SAFE_RELEASE( objMesh );
}

HRESULT Mesh::LoadModelsFromXFile(LPDIRECT3DDEVICE9 theDevice, const char *aString , const char *aTexturePath)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;	/*一時バッファ用*/
	
	//=========================================================================
	// Xファイルの読み込み。
    if( FAILED( D3DXLoadMeshFromX(	aString, D3DXMESH_SYSTEMMEM , theDevice, NULL, 
									&pD3DXMtrlBuffer, NULL,
									&objNumOfMaterials , &objMesh ) ) )
	{
		char cautionString[STR_LENGTH];
		sprintf( cautionString , "Xファイル『%s』が見つかりませんでした。", aString);
		MessageBox(NULL, cautionString, WINDOW_TITLE , MB_OK);
		return(E_FAIL);
	}

	// pD3DXMtrlBufferから、マテリアルやテクスチャの名称などの情報を取得する。
	D3DXMATERIAL* d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	objMeshMaterials	= new D3DMATERIAL9[objNumOfMaterials];
	objMeshTextures	= new LPDIRECT3DTEXTURE9[objNumOfMaterials];

	for(DWORD i0=0; i0 < objNumOfMaterials ; i0++ )
	{
		// 質感（material）のコピー
		objMeshMaterials[i0] = d3dxMaterials[i0].MatD3D;

		// 質感（material）に環境光を設定する（D3DXが何もしないため）
		objMeshMaterials[i0].Ambient = objMeshMaterials[i0].Diffuse;

		objMeshTextures[i0] = NULL;
		if( d3dxMaterials[i0].pTextureFilename != NULL && lstrlen(d3dxMaterials[i0].pTextureFilename) > 0 )
		{
			//テクスチャパスの連結
			TCHAR strTexture[MAX_PATH];
			DWORD aPathLength = lstrlen( aTexturePath );
			lstrcpyn( strTexture, aTexturePath, MAX_PATH );
			lstrcpyn( strTexture + aPathLength, d3dxMaterials[i0].pTextureFilename, MAX_PATH - aPathLength );

			if(FAILED( D3DXCreateTextureFromFile(	theDevice,
													strTexture ,
													&objMeshTextures[i0])))
			{
				char cautionString[STR_LENGTH];

				sprintf( cautionString , "Xファイル『%s』で指定されたテクスチャマップファイル『%s』が見つかりませんでした。", aString ,d3dxMaterials[i0].pTextureFilename );
				MessageBox(NULL, cautionString, WINDOW_TITLE , MB_OK);
				return( E_FAIL );
			}
		}
	}

	// 質感（material）バッファの開放。
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//バウンディングスフィアの計算
	CalcBoundingSphere();
	CalcBoundingBox();

	return(S_OK);
}

HRESULT	loadTextureFromImageFiles( LPCTSTR aFileName , LPDIRECT3DTEXTURE9* aTexture , LPCTSTR aSearchPath , LPDIRECT3DDEVICE9 theDevice )
{
	// テクスチャの生成
	// これもまずカレントフォルダを探し、見つからない場合は「models」フォルダ内を探す
	if( FAILED( D3DXCreateTextureFromFile(	theDevice,aFileName,aTexture ) ) )
	{
		// もしカレントディレクトリにファイルが無かったら、modelsフォルダを確かめる。
		// バックスラッシュ(\)が二つ重なっているのは、エスケープシーケンスが有効になっているため
		TCHAR strTexture[MAX_PATH];

		DWORD aPathLength = lstrlen( aSearchPath );

		lstrcpyn( strTexture, aSearchPath, MAX_PATH );
		lstrcpyn( strTexture + aPathLength, aFileName, MAX_PATH - aPathLength );

		// テクスチャが無かったら、ひとつ上のディレクトリを確かめる。
		if( FAILED(D3DXCreateTextureFromFile( theDevice, strTexture, aTexture )) ){ return(E_FAIL); }
	}

	return( S_OK );
}
HRESULT Mesh::CalcBoundingSphere(){
	HRESULT hr;

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;        //頂点バッファ
	void*                    pVertices = NULL;  //メモリバッファのポインタ



	//頂点バッファ内のデータを取得する
	if(FAILED(hr = objMesh->GetVertexBuffer(&pVB)))
	{
		return hr;
	}

	if(FAILED(hr = pVB->Lock(
		0,              //全体をロック
		0,              //全体をロック
		&pVertices,
		0)))
	{
		SAFE_RELEASE(pVB);
		return hr;
	}

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		objMesh->GetNumVertices(),    //頂点数
		D3DXGetFVFVertexSize(objMesh->GetFVF()),
		&objCenter, &objRadius);  //返される中心座標と半径

	//ロック解除
	pVB->Unlock();
	SAFE_RELEASE(pVB);

    return hr;

}
HRESULT Mesh::CalcBoundingBox(){
	HRESULT hr;

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;        //頂点バッファ
	void*                    pVertices = NULL;  //メモリバッファのポインタ



	//頂点バッファ内のデータを取得する
	if(FAILED(hr = objMesh->GetVertexBuffer(&pVB)))
	{
		return hr;
	}

	if(FAILED(hr = pVB->Lock(
		0,              //全体をロック
		0,              //全体をロック
		&pVertices,
		0)))
	{
		SAFE_RELEASE(pVB);
		return hr;
	}

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		objMesh->GetNumVertices(),    //頂点数
		D3DXGetFVFVertexSize(objMesh->GetFVF()),
		&objMin, &objMax);  //返される中心座標と半径

	//ロック解除
	pVB->Unlock();
	SAFE_RELEASE(pVB);

    return hr;

}

void Mesh::GetBoundingSphere(D3DXVECTOR3* center,float* radius){
	*center = objCenter;
	*radius = objRadius;
}
void Mesh::GetBoundingBox(D3DXVECTOR3 *min, D3DXVECTOR3 *max){
	*min = objMin;
	*max = objMax;
}
HRESULT Mesh::FindVerticesOnPoly(DWORD index, D3DXVECTOR3* vertices){

	DWORD NumBytes = objMesh->GetNumBytesPerVertex();
	DWORD NumVertices = objMesh->GetNumVertices();
	DWORD NumFaces = objMesh->GetNumFaces();

	WORD* pwPoly = NULL;
	objMesh->LockIndexBuffer(D3DLOCK_READONLY,(VOID**)&pwPoly);

	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	BYTE* pbVertices = NULL;
	FLOAT* pfVertices = NULL;
	objMesh->GetVertexBuffer(&VB);
	if(SUCCEEDED(VB->Lock(0,0,(VOID**)&pbVertices,0))){

		pfVertices = (FLOAT*)&pbVertices[NumBytes * pwPoly[index * 3]];

		vertices[0].x = pfVertices[0];
		vertices[0].y = pfVertices[1];
		vertices[0].z = pfVertices[2];

		pfVertices = (FLOAT*)&pbVertices[NumBytes * pwPoly[index * 3+1]];

		vertices[1].x = pfVertices[0];
		vertices[1].y = pfVertices[1];
		vertices[1].z = pfVertices[2];

		pfVertices = (FLOAT*)&pbVertices[NumBytes * pwPoly[index * 3+2]];

		vertices[2].x = pfVertices[0];
		vertices[2].y = pfVertices[1];
		vertices[2].z = pfVertices[2];

		VB->Unlock();
		objMesh->UnlockIndexBuffer();
	}
	
//	MyOutputDebugString("%f,%f,%f\n",vertices[0].x,vertices[0].y,vertices[0].z);
	return S_OK;
}