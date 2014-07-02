#include "Mesh.h"
#include "Common.h"

Mesh::Mesh()
:objMeshMaterials(NULL),
objMeshTextures(NULL),
objMesh(NULL)
{}
Mesh::~Mesh(){

		/*���b�V���}�e���A���̉��*/
		SAFE_DELETE_ARRAY(objMeshMaterials);

		/*�e�N�X�`���̉��*/
		if(objMeshTextures )
		{
			for(DWORD j0 = 0; j0 < objNumOfMaterials ; j0++ ){ SAFE_RELEASE(objMeshTextures[j0] );}
			SAFE_DELETE_ARRAY(objMeshTextures );
		}
		
		/*���b�V���̊J��*/
		SAFE_RELEASE( objMesh );
}

HRESULT Mesh::LoadModelsFromXFile(LPDIRECT3DDEVICE9 theDevice, const char *aString , const char *aTexturePath)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;	/*�ꎞ�o�b�t�@�p*/
	
	//=========================================================================
	// X�t�@�C���̓ǂݍ��݁B
    if( FAILED( D3DXLoadMeshFromX(	aString, D3DXMESH_SYSTEMMEM , theDevice, NULL, 
									&pD3DXMtrlBuffer, NULL,
									&objNumOfMaterials , &objMesh ) ) )
	{
		char cautionString[STR_LENGTH];
		sprintf( cautionString , "X�t�@�C���w%s�x��������܂���ł����B", aString);
		MessageBox(NULL, cautionString, WINDOW_TITLE , MB_OK);
		return(E_FAIL);
	}

	// pD3DXMtrlBuffer����A�}�e���A����e�N�X�`���̖��̂Ȃǂ̏����擾����B
	D3DXMATERIAL* d3dxMaterials	= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	objMeshMaterials	= new D3DMATERIAL9[objNumOfMaterials];
	objMeshTextures	= new LPDIRECT3DTEXTURE9[objNumOfMaterials];

	for(DWORD i0=0; i0 < objNumOfMaterials ; i0++ )
	{
		// �����imaterial�j�̃R�s�[
		objMeshMaterials[i0] = d3dxMaterials[i0].MatD3D;

		// �����imaterial�j�Ɋ�����ݒ肷��iD3DX���������Ȃ����߁j
		objMeshMaterials[i0].Ambient = objMeshMaterials[i0].Diffuse;

		objMeshTextures[i0] = NULL;
		if( d3dxMaterials[i0].pTextureFilename != NULL && lstrlen(d3dxMaterials[i0].pTextureFilename) > 0 )
		{
			//�e�N�X�`���p�X�̘A��
			TCHAR strTexture[MAX_PATH];
			DWORD aPathLength = lstrlen( aTexturePath );
			lstrcpyn( strTexture, aTexturePath, MAX_PATH );
			lstrcpyn( strTexture + aPathLength, d3dxMaterials[i0].pTextureFilename, MAX_PATH - aPathLength );

			if(FAILED( D3DXCreateTextureFromFile(	theDevice,
													strTexture ,
													&objMeshTextures[i0])))
			{
				char cautionString[STR_LENGTH];

				sprintf( cautionString , "X�t�@�C���w%s�x�Ŏw�肳�ꂽ�e�N�X�`���}�b�v�t�@�C���w%s�x��������܂���ł����B", aString ,d3dxMaterials[i0].pTextureFilename );
				MessageBox(NULL, cautionString, WINDOW_TITLE , MB_OK);
				return( E_FAIL );
			}
		}
	}

	// �����imaterial�j�o�b�t�@�̊J���B
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//�o�E���f�B���O�X�t�B�A�̌v�Z
	CalcBoundingSphere();
	CalcBoundingBox();

	return(S_OK);
}

HRESULT	loadTextureFromImageFiles( LPCTSTR aFileName , LPDIRECT3DTEXTURE9* aTexture , LPCTSTR aSearchPath , LPDIRECT3DDEVICE9 theDevice )
{
	// �e�N�X�`���̐���
	// ������܂��J�����g�t�H���_��T���A������Ȃ��ꍇ�́umodels�v�t�H���_����T��
	if( FAILED( D3DXCreateTextureFromFile(	theDevice,aFileName,aTexture ) ) )
	{
		// �����J�����g�f�B���N�g���Ƀt�@�C��������������Amodels�t�H���_���m���߂�B
		// �o�b�N�X���b�V��(\)����d�Ȃ��Ă���̂́A�G�X�P�[�v�V�[�P���X���L���ɂȂ��Ă��邽��
		TCHAR strTexture[MAX_PATH];

		DWORD aPathLength = lstrlen( aSearchPath );

		lstrcpyn( strTexture, aSearchPath, MAX_PATH );
		lstrcpyn( strTexture + aPathLength, aFileName, MAX_PATH - aPathLength );

		// �e�N�X�`��������������A�ЂƂ�̃f�B���N�g�����m���߂�B
		if( FAILED(D3DXCreateTextureFromFile( theDevice, strTexture, aTexture )) ){ return(E_FAIL); }
	}

	return( S_OK );
}
HRESULT Mesh::CalcBoundingSphere(){
	HRESULT hr;

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;        //���_�o�b�t�@
	void*                    pVertices = NULL;  //�������o�b�t�@�̃|�C���^



	//���_�o�b�t�@���̃f�[�^���擾����
	if(FAILED(hr = objMesh->GetVertexBuffer(&pVB)))
	{
		return hr;
	}

	if(FAILED(hr = pVB->Lock(
		0,              //�S�̂����b�N
		0,              //�S�̂����b�N
		&pVertices,
		0)))
	{
		SAFE_RELEASE(pVB);
		return hr;
	}

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		objMesh->GetNumVertices(),    //���_��
		D3DXGetFVFVertexSize(objMesh->GetFVF()),
		&objCenter, &objRadius);  //�Ԃ���钆�S���W�Ɣ��a

	//���b�N����
	pVB->Unlock();
	SAFE_RELEASE(pVB);

    return hr;

}
HRESULT Mesh::CalcBoundingBox(){
	HRESULT hr;

	LPDIRECT3DVERTEXBUFFER9  pVB = NULL;        //���_�o�b�t�@
	void*                    pVertices = NULL;  //�������o�b�t�@�̃|�C���^



	//���_�o�b�t�@���̃f�[�^���擾����
	if(FAILED(hr = objMesh->GetVertexBuffer(&pVB)))
	{
		return hr;
	}

	if(FAILED(hr = pVB->Lock(
		0,              //�S�̂����b�N
		0,              //�S�̂����b�N
		&pVertices,
		0)))
	{
		SAFE_RELEASE(pVB);
		return hr;
	}

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		objMesh->GetNumVertices(),    //���_��
		D3DXGetFVFVertexSize(objMesh->GetFVF()),
		&objMin, &objMax);  //�Ԃ���钆�S���W�Ɣ��a

	//���b�N����
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