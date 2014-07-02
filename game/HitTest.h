#ifndef __HIT_TEST_H__
#define __HIT_TEST_H__

#include "MeshObject.h"
#include "AnimObject.h"

//単純な境界球による当たり判定

//（アニメーション）メッシュと（アニメーション）メッシュ
bool HitTest(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir);
bool HitTestB(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir);
bool HitTest(MeshObjectBase* objA,D3DXVECTOR3* posB,D3DXVECTOR3* dir);
bool PushOutOfTerritory(MeshObjectBase* objA,MeshObjectBase* objB);

//攻撃の当たり判定

//アニメーションメッシュの部位とアニメーションメッシュ全体
bool HitTest(AnimObject* attackObj, int attack_id, AnimObject* defenseObj);
//アニメーションメッシュの部位とメッシュ全体
bool HitTest(AnimObject* attackObj,int attack_id,MeshObject* defenseObj);
//アニメーションメッシュの部位とアニメーションメッシュの部位
bool HitTest(AnimObject* attackObj, int attack_id,AnimObject* defenseObj, int defense_id);


#endif	/*__HIT_TEST_H__*/