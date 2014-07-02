#ifndef __HIT_TEST_H__
#define __HIT_TEST_H__

#include "MeshObject.h"
#include "AnimObject.h"

//�P���ȋ��E���ɂ�铖���蔻��

//�i�A�j���[�V�����j���b�V���Ɓi�A�j���[�V�����j���b�V��
bool HitTest(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir);
bool HitTestB(MeshObjectBase* objA,MeshObjectBase* objB,D3DXVECTOR3* dir);
bool HitTest(MeshObjectBase* objA,D3DXVECTOR3* posB,D3DXVECTOR3* dir);
bool PushOutOfTerritory(MeshObjectBase* objA,MeshObjectBase* objB);

//�U���̓����蔻��

//�A�j���[�V�������b�V���̕��ʂƃA�j���[�V�������b�V���S��
bool HitTest(AnimObject* attackObj, int attack_id, AnimObject* defenseObj);
//�A�j���[�V�������b�V���̕��ʂƃ��b�V���S��
bool HitTest(AnimObject* attackObj,int attack_id,MeshObject* defenseObj);
//�A�j���[�V�������b�V���̕��ʂƃA�j���[�V�������b�V���̕���
bool HitTest(AnimObject* attackObj, int attack_id,AnimObject* defenseObj, int defense_id);


#endif	/*__HIT_TEST_H__*/