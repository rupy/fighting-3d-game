#include "HitTest.h"
#include "Math.h"
#include "VectorTest.h"

bool HitTest(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir){

	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	objA->GetMesh()->GetBoundingSphere(&centerA,&radiusA);

	objB->GetMesh()->GetBoundingSphere(&centerB,&radiusB);

	//攻撃判定と防御判定の中心間の距離を求める
	D3DXVECTOR3 dist=( centerA + objA->Pos + objA->Velo ) - ( centerB + objB->Pos + objB->Velo );
	//攻撃間の防御判定の中心間の距離が攻撃判定の半径＋防御判定の半径以下ならば接触したとみなす

	if (D3DXVec3Length(&dist)<=radiusA+radiusB){
		D3DXPLANE plane;
		D3DXPlaneFromPointNormal(&plane,&(centerA+objA->Pos + objA->Velo),&dist);//Aの境界球の接平面を求める
		D3DXVECTOR3 normal = D3DXVECTOR3(plane.a,plane.b,plane.c);//当たった面の法線を調べる
		D3DXVECTOR3 C,P;
		//MyOutputDebugString( _T("<dir:%f,%f,%f>\n"),normal.x,normal.y,normal.z);
		D3DXVec3Normalize(&normal,&-normal);
		D3DXVECTOR3 line = (centerB + objB->Pos + objB->Velo) + normal;//接平面に向かう垂直ベクトル
		PlaneLine_Point(&P,&plane,&(centerB+objB->Pos + objB->Velo),&line);//垂直ベクトルとの接点ル
		PlaneLine_Point(&C,&plane,&(centerB+objB->Pos + objB->Velo),&( centerA + objA->Pos + objA->Velo ));//垂直ベクトルとの接点

		//D3DXVec3Normalize(&dist,&dist);
		*dir = C-P + -dist;
		VectorTest::SetDir(*dir);
		return true;
	}
	dir = NULL;
	return false;
}
bool HitTestB(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir){

	D3DXVECTOR3 minA,minB;
	D3DXVECTOR3 maxA,maxB;
	objA->GetMesh()->GetBoundingBox(&minA,&maxA);

	objB->GetMesh()->GetBoundingBox(&minB,&maxB);


	return true ;

	//これは未完成
}
bool HitTest(MeshObjectBase* objA,D3DXVECTOR3* posB,D3DXVECTOR3* dir){

	D3DXVECTOR3 centerA;
	float radiusA;

	objA->GetMesh()->GetBoundingSphere(&centerA,&radiusA);


	//攻撃判定と防御判定の中心間の距離を求める
	D3DXVECTOR3 dist=( centerA + objA->Pos ) - (*posB );
	*dir = dist;
	//攻撃間の防御判定の中心間の距離が攻撃判定の半径＋防御判定の半径以下ならば接触したとみなす

	if (D3DXVec3Length(&dist)<=radiusA){
		return true;
	}
	return false;
}

bool PushOutOfTerritory(MeshObjectBase* objA,MeshObjectBase* objB){

	D3DXVECTOR3 dir;
	if(HitTest(objA,objB,&dir)){
		if(fabs(dir.y) > 2.0f ){
			//D3DXVec3Normalize(&dir,&dir);
			objB->Velo.x = dir.x*1.0f;
			objB->Velo.z = dir.z*1.0f;
		}else{
			objB->Velo.x = 0;
			objB->Velo.z = 0;
		}
		return true;
	}
	return false;
}
bool HitTest(AnimObject* attackObj,int attack_id,AnimObject* defenseObj){

	Hit* hit_attack= &attackObj->GetMesh()->original->HitAttack[attack_id];
	
	//攻撃判定のあるフレームの合成行列を取得する
	D3DXMATRIXA16& am=attackObj->GetMesh()->GetFrame(hit_attack->FrameIndex)->CombinedMatrix;
	
	//合成行列から、攻撃判定の中心座標を求める
	D3DXVECTOR3 apos(am._41, am._42, am._43);
	//防御判定のすべての部位についてループする
	for (int i=0, n=sizeof(defenseObj->GetMesh()->original->HitDefense)/sizeof(Hit); i<n; i++) {

		//防御判定があるフレームの合成行列を取得する
		D3DXMATRIXA16& dm = defenseObj->GetMesh()->GetFrame( defenseObj->GetMesh()->original->HitDefense[i].FrameIndex )->CombinedMatrix;
		//合成行列から、防御判定の中心座標を求める
		D3DXVECTOR3 dpos(dm._41, dm._42, dm._43);

		//攻撃判定と防御判定の中心間の距離を求める
		D3DXVECTOR3 dist = apos - dpos;

		//攻撃間の防御判定の中心間の距離が攻撃判定の半径＋防御判定の半径以下ならば接触したとみなす
		if (D3DXVec3Length(&dist) <= hit_attack->Radius + defenseObj->GetMesh()->original->HitDefense[i].Radius){
			return true;
		}
	}
	return false;
}
bool HitTest(AnimObject* attackObj,int attack_id,MeshObject* defenseObj){


	Hit* hit_attack= &attackObj->GetMesh()->original->HitAttack[attack_id];
	
	//攻撃判定のあるフレームの合成行列を取得する
	D3DXMATRIXA16& am=attackObj->GetMesh()->GetFrame(hit_attack->FrameIndex)->CombinedMatrix;
	
	//合成行列から、攻撃判定の中心座標を求める
	D3DXVECTOR3 apos(am._41, am._42, am._43);

	D3DXVECTOR3 dpos;
	float radius;

	defenseObj->GetMesh()->GetBoundingSphere(&dpos,&radius);

	//攻撃判定と防御判定の中心間の距離を求める
	D3DXVECTOR3 dist=apos-(dpos+defenseObj->Pos);
	MyOutputDebugString( _T("<dist:%f,%f,%f>\n"),dist.x,dist.y,dist.z);
	//攻撃間の防御判定の中心間の距離が攻撃判定の半径＋防御判定の半径以下ならば接触したとみなす
	if (D3DXVec3Length(&dist)<=hit_attack->Radius+radius){
		return true;
	}
	return false;
}
bool HitTest(AnimObject* attackObj, int attack_id,AnimObject* defenseObj, int defense_id){

	Hit* hit_attack= &attackObj->GetMesh()->original->HitAttack[attack_id];
	Hit* hit_defense= &attackObj->GetMesh()->original->HitDefense[defense_id];

	//攻撃判定のあるフレームの合成行列を取得する
	D3DXMATRIXA16& am=attackObj->GetMesh()->GetFrame(hit_attack->FrameIndex)->CombinedMatrix;

	//合成行列から、攻撃判定の中心座標を求める
	D3DXVECTOR3 apos(am._41, am._42, am._43);

	//防御判定があるフレームの合成行列を取得する
	D3DXMATRIXA16& dm = defenseObj->GetMesh()->GetFrame( hit_defense->FrameIndex )->CombinedMatrix;
	//合成行列から、防御判定の中心座標を求める
	D3DXVECTOR3 dpos(dm._41, dm._42, dm._43);

	//攻撃判定と防御判定の中心間の距離を求める
	D3DXVECTOR3 dist = apos - dpos;

	//攻撃間の防御判定の中心間の距離が攻撃判定の半径＋防御判定の半径以下ならば接触したとみなす
	if (D3DXVec3Length(&dist) <= hit_attack->Radius + hit_defense->Radius){
		return true;
	}
	return false;
}
