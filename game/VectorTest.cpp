#include "VectorTest.h"

VectorTest* VectorTest::This = NULL;
float VectorTest::veclong = 10.0f;

VectorTest::VectorTest(vector<Effect*>& eff)
:MeshObject(eff)
{
	this->upVec = D3DXVECTOR3(1,1,1);
	this->lookatPt = D3DXVECTOR3(0,0,1);
	LoadMeshFromXFile(VECTOR_MODEL_FILENAME);

	This = this;
}
void VectorTest::SetDir(D3DXVECTOR3 dir){

	This->lookatPt = -dir;
	//Scale.x = D3DXVec3Length(&dir)*10.0f;
	//Scale.y = D3DXVec3Length(&dir)*10.0f;
	This->Scale.z = D3DXVec3Length(&dir)*veclong;
}
void VectorTest::Shadow(){
	
	D3DXMATRIXA16 matWorld = BillboardMatrixSetting();
	if(FAILED(rEff[1]->SetMatrix(&matWorld,"g_world"))){
		return;
	}

	if(FAILED(rEff[1]->SetTechnique())){
		return;
	}

	rEff[1]->pFX->CommitChanges();

	
	UINT numPass;

	if(FAILED(rEff[1]->pFX->Begin(&numPass, 0))){
		return;
	}


	for(DWORD i=0; i < MeshType->objNumOfMaterials; i++){

		rEff[1]->pFX->BeginPass(0);
		MeshType->objMesh->DrawSubset(i);
		rEff[1]->pFX->EndPass();
	}

	rEff[1]->pFX->End();
	
	
}
void VectorTest::Appear(){

	D3DXMATRIXA16 matWorld = BillboardMatrixSetting();

	if(FAILED(rEff[0]->SetTechnique())){
		return;
	}

	if(FAILED(rEff[0]->SetMatrix(&matWorld,"g_world"))){
		return;
	}

	D3DXMATRIX matInv;
	D3DXMatrixInverse(&matInv,NULL,&matWorld);


	if(FAILED(rEff[0]->SetMatrix(&matInv,"g_world_inv"))){
		return;
	}


	UINT numPass;

	if(FAILED(rEff[0]->pFX->Begin(&numPass, 0))){
		return;
	}

	for(DWORD i=0; i < MeshType->objNumOfMaterials; i++){

		if(MeshType->objMeshTextures[i] != 0){
			rEff[0]->pFX->BeginPass(0);
		}
		else{
			rEff[0]->pFX->BeginPass(1);
		}
		
		D3DXVECTOR4		tmpColor;
		rEff[0]->SetTexture(MeshType->objMeshTextures[i],"g_texture");
		tmpColor.x = MeshType->objMeshMaterials[i].Emissive.r;
		tmpColor.y = MeshType->objMeshMaterials[i].Emissive.g;
		tmpColor.z = MeshType->objMeshMaterials[i].Emissive.b;
		tmpColor.w = 1.0f;
		if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color0"))){
			return;
		}

		tmpColor.x = MeshType->objMeshMaterials[i].Diffuse.r;
		tmpColor.y = MeshType->objMeshMaterials[i].Diffuse.g;
		tmpColor.z = MeshType->objMeshMaterials[i].Diffuse.b;
		tmpColor.w = 1.0f;
		if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color1"))){
			return;
		}
		
		if(FAILED(rEff[0]->pFX->CommitChanges())){
			return;
		}

		MeshType->objMesh->DrawSubset(i);

		rEff[0]->pFX->EndPass();
	}

	rEff[0]->pFX->End();
}