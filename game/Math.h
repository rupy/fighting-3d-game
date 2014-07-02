#ifndef __MATH_H__
#define __MATH_H__

#include <d3dx9.h>
//自分で用意した数学関数など

//平面と直線の交点を調べる
D3DXVECTOR3* PlaneLine_Point(D3DXVECTOR3* point,const D3DXPLANE* plane,const D3DXVECTOR3* v1,const D3DXVECTOR3* v2);

#endif	/*__MATH_H__*/