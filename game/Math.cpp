#include "Math.h"


D3DXVECTOR3* PlaneLine_Point(D3DXVECTOR3* point,const D3DXPLANE* plane,const D3DXVECTOR3* v1,const D3DXVECTOR3* v2){

	//パラメトリック方程式を導く
	float t = 
		-((plane->a * v2->x)+(plane->b * v2->y)+(plane->c * v2->z)+plane->d)/
		( ((plane->a * v1->x)+(plane->b * v1->y)+(plane->c * v1->z)) - ((plane->a * v2->x)+(plane->b * v2->y)+(plane->c * v2->z)) );

//	if(t>=0 && t<=1.0f){
		//無限平面と直線が交差していれば交点を計算をする
		point->x = t * v1->x + (1-t) * v2->x;
		point->y = t * v1->y + (1-t) * v2->y;
		point->z = t * v1->z + (1-t) * v2->z;
//	}else{
		//交差していなかったらNULLを返す
//		point = NULL;
//	}
		return point;
}