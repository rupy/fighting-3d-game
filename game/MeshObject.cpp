#include "MeshObject.h"
#include "Error.h"
#include "Math.h"
#include "VectorTest.h"

MeshObject::MeshObject(vector<Effect*>& eff,
					   Mesh* type,
					   D3DXVECTOR3 position, float Y,float R,float P)
					   :MeshObjectBase(eff,position,Y,R,P),
					   MeshType(type)
{
}
MeshObject::~MeshObject(){

	if(myMesh){
		SAFE_DELETE(MeshType);
	}
}

void MeshObject::Shadow(){
	
	D3DXMATRIXA16 matWorld = MatrixSetting();
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
void MeshObject::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();

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

	if(FAILED(rEff[0]->SetBool(&use_shadow,"g_use_shadow"))){
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
void MeshObject::PushOutOfTerritory(Object* obj){

}
bool MeshObject::WallHit(Object* obj,float minDist,float stayHeight){

	D3DXVECTOR3 start,end,dir;

	start = end = obj->Pos;
	start.x -= obj->Velo.x;
	start.z -= obj->Velo.z;
	start.y -= stayHeight;
	end.y -= stayHeight;

	D3DXMATRIX world,worldInv;
	D3DXMatrixIdentity(&worldInv);
	world = MatrixSetting();
	D3DXMatrixInverse(&worldInv,NULL,&world);
	D3DXVec3TransformCoord(&start,&start,&worldInv);
	D3DXVec3TransformCoord(&end,&end,&worldInv);

	dir = end - start;

	float dist=0;
	BOOL hit=false;
	DWORD faceIndex=0;
	float U=0,V=0;
	D3DXIntersect(
		MeshType->objMesh,
		&start,							//レイの始点
		&dir,							//レイの方向
		&hit,							//ヒットしているかどうか
		&faceIndex,						//当たった面のインデックス
		&U,&V,							//当たった面の重心座標
		&dist,							//当たった面までの距離
		NULL,NULL);

	if(hit == 1){
		//勾配処理

		D3DXVECTOR3 vertex[3];
		MeshType->FindVerticesOnPoly(faceIndex,vertex);

		//オブジェクトから正面へのレイの交点を調べる
		D3DXVECTOR3 C = vertex[0]+ U *(vertex[1]-vertex[0])+ V*(vertex[2] - vertex[0]);
		D3DXPLANE plane;
		D3DXPlaneFromPoints(&plane,&vertex[0],&vertex[1],&vertex[2]);
		D3DXVECTOR3 normal = D3DXVECTOR3(plane.a,plane.b,plane.c);//当たった面の法線を調べる
		//ポリゴンが裏側の時には当たり判定せずに帰る。
		if(D3DXVec3Dot(&normal,&dir)>0){
			//MyOutputDebugString("<裏側>dir[%f,%f,%f]normal[%f,%f,%f]\n",dir.x,dir.y,dir.z,normal.x,normal.y,normal.z);
			return false;
		}
		//else{MyOutputDebugString(">表側<dir[%f,%f,%f]normal[%f,%f,%f]\n",dir.x,dir.y,dir.z,normal.x,normal.y,normal.z);}

		D3DXVECTOR3 P;
		D3DXVec3Normalize(&normal,&-normal);
		D3DXVECTOR3 line = start + normal;
		PlaneLine_Point(&P,&plane,&start,&line);
		D3DXVec3TransformCoord(&C,&C,&world);//交点をワールド座標に変換する
		D3DXVec3TransformCoord(&P,&P,&world);//交点をワールド座標に変換する
		D3DXVECTOR3 slide = C - P;
		float an= D3DXVec3Length(&slide);

		D3DXVec3Normalize(&slide,&slide);
		//VectorTest::SetDir(slide);
		//壁との距離が一定以下なら壁に当たっていると判断して、壁ずりをする。
		if(dist < minDist){
			obj->Velo.x = slide.x * WALL_SLIP;
			obj->Velo.z = slide.z * WALL_SLIP;
			return true;
			obj->wall = true;
		}else{
			//VectorTest::SetDir(D3DXVECTOR3(0,0,0));
		}
	}
	obj->wall = false;
	return false;
}
bool MeshObject::LandHit(Object* obj,bool slope,float stayHeight){

	bool down;
	//上に移動していたら地面との当たり判定はしない
	if(obj->Velo.y > 0.0f){//↑
		down = false;
		slope = false;
		obj->land = false;
	}else{//↓
		down = true;
		obj->top = false;
	}

	if(down){
		obj->Velo.y += MAX_OF_NEXT_HEIGHT;//少し上からレイを落とす
	}else{
		obj->Velo.y -= MAX_OF_NEXT_HEIGHT;//少し上からレイを落とす
	}

	D3DXVECTOR3 start,end,dir;

	start = end = obj->Pos + obj->Velo;//移動後の場所
	if(down){
		end.y -= 1.0f;
	}else{
		end.y += 1.0f;
	}


	//地形のローカル座標に変換する
	D3DXMATRIX world,worldInv,a;
	D3DXMatrixIdentity(&worldInv);
	world = MatrixSetting();
	D3DXMatrixInverse(&worldInv,NULL,&world);
	D3DXVec3TransformCoord(&start,&start,&worldInv);
	D3DXVec3TransformCoord(&end,&end,&worldInv);

	dir = end - start;

	//MyOutputDebugString("S[%f,%f,%f]\n",start.x,start.y,start.z);
	//MyOutputDebugString("E[%f,%f,%f]\n",end.x,end.y,end.z);
	//MyOutputDebugString("D[%f,%f,%f]\n",dir.x,dir.y,dir.z);
	float dist=0;
	BOOL hit=false;
	DWORD faceIndex=0;
	float U=0,V=0;

	D3DXIntersect(
		MeshType->objMesh,
		&start,							//レイの始点
		&dir,							//レイの方向
		&hit,							//ヒットしているかどうか
		&faceIndex,						//当たった面のインデックス
		&U,&V,							//当たった面の重心座標
		&dist,							//当たった面までの距離
		NULL,NULL);

	dist -= stayHeight;

	if(hit == 1){

		if(slope){
			//勾配処理

			D3DXVECTOR3 vertex[3];
			MeshType->FindVerticesOnPoly(faceIndex,vertex);//当たった面の３頂点を調べる

			D3DXVECTOR3 C = vertex[0]+ U *(vertex[1]-vertex[0])+ V*(vertex[2] - vertex[0]);//レイの交点を調べる
			D3DXPLANE plane;
			D3DXPlaneFromPoints(&plane,&vertex[0],&vertex[1],&vertex[2]);//面の平面方程式
			D3DXVECTOR3 normal = D3DXVECTOR3(-plane.a,-plane.b,-plane.c);//面の法線
			D3DXVECTOR3 P;
			D3DXVec3Normalize(&normal,&normal);
			D3DXVECTOR3 line = start + normal;
			PlaneLine_Point(&P,&plane,&start,&line);
			D3DXVec3TransformCoord(&C,&C,&world);//交点をワールド座標に変換する
			D3DXVec3TransformCoord(&P,&P,&world);//交点をワールド座標に変換する
			//MyOutputDebugString("O[%f,%f,%f]\n",obj->Pos.x,obj->Pos.y,obj->Pos.z);
			//MyOutputDebugString("C[%f,%f,%f]\n",C.x,C.y,C.z);
			//MyOutputDebugString("P[%f,%f,%f]\n",P.x,P.y,P.z);
			D3DXVECTOR3 slide = C - P;
			if(D3DXVec3Length(&slide)>0.0001f){
				//VectorTest::SetDir(slide);
				float an= D3DXVec3Length(&slide);
				if(an >SLIDE_MIN_ANGLE){
					obj->Velo += slide *SLIDE_HEAVIY;
				}
				//MyOutputDebugString("S[%f,%f,%f]%f\n",slide.x,slide.y,slide.z);
				//VectorTest::SetDir(slide);
			}
		}

		//落下処理

		if(down && dist<MAX_OF_NEXT_HEIGHT){//距離＜最大到達標高
			//接地
			obj->Velo.y -= dist;
			obj->land = true;
			return true;
		}
		if(!down && dist<MAX_OF_NEXT_HEIGHT){//距離＜最大到達標高
			//接地
			obj->Velo.y += dist;
			obj->top = true;
			return true;
		}
	}
	if(down){
		obj->Velo.y -= MAX_OF_NEXT_HEIGHT;//元に戻す
		obj->land = false;
	}else{
		obj->Velo.y += MAX_OF_NEXT_HEIGHT;//元に戻す
		obj->top = false;
	}
	
	return false;
}
bool MeshObject::Landing(Object* obj,float stayHeight){

	obj->top = false;

	D3DXVECTOR3 start,end,dir;

	start = end = obj->Pos + obj->Velo;//移動後の場所
	end.y -= 1.0f;

	//地形のローカル座標に変換する
	D3DXMATRIX world,worldInv,a;
	D3DXMatrixIdentity(&worldInv);
	world = MatrixSetting();
	D3DXMatrixInverse(&worldInv,NULL,&world);
	D3DXVec3TransformCoord(&start,&start,&worldInv);
	D3DXVec3TransformCoord(&end,&end,&worldInv);

	dir = end - start;

	float dist=0;
	BOOL hit=false;
	DWORD faceIndex=0;
	float U=0,V=0;

	D3DXIntersect(
		MeshType->objMesh,
		&start,							//レイの始点
		&dir,							//レイの方向
		&hit,							//ヒットしているかどうか
		&faceIndex,						//当たった面のインデックス
		&U,&V,							//当たった面の重心座標
		&dist,							//当たった面までの距離
		NULL,NULL);

	dist -= stayHeight;

	if(hit == 1){

		//落下処理

		//接地
		obj->Pos.y -= dist;
		obj->land = true;
		return true;
	}
	return false;
}
HRESULT MeshObject::LoadMeshFromXFile(const char* filename,const char *aTexturePath){
	if(!MeshType && !myMesh){
		MeshType = new Mesh;
		if(FAILED(MeshType->LoadModelsFromXFile(GetDevice(),filename, aTexturePath))){
			DisplayErrMsg("モデルの読み込みに失敗しました");
			return(E_FAIL);
		}
	}else{
		DisplayErrMsg("すでにモデルが登録されているアニメーションメッシュにモデルを読み込もうとしました。");
		return E_FAIL;
	}
	myMesh = true;
	return S_OK;
}

Mesh* MeshObject::GetMesh(){
	return MeshType;
}