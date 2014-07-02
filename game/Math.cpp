#include "Math.h"


D3DXVECTOR3* PlaneLine_Point(D3DXVECTOR3* point,const D3DXPLANE* plane,const D3DXVECTOR3* v1,const D3DXVECTOR3* v2){

	//�p�����g���b�N�������𓱂�
	float t = 
		-((plane->a * v2->x)+(plane->b * v2->y)+(plane->c * v2->z)+plane->d)/
		( ((plane->a * v1->x)+(plane->b * v1->y)+(plane->c * v1->z)) - ((plane->a * v2->x)+(plane->b * v2->y)+(plane->c * v2->z)) );

//	if(t>=0 && t<=1.0f){
		//�������ʂƒ������������Ă���Ό�_���v�Z������
		point->x = t * v1->x + (1-t) * v2->x;
		point->y = t * v1->y + (1-t) * v2->y;
		point->z = t * v1->z + (1-t) * v2->z;
//	}else{
		//�������Ă��Ȃ�������NULL��Ԃ�
//		point = NULL;
//	}
		return point;
}