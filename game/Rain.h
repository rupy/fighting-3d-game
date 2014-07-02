#ifndef __RAIN_H__
#define __RAIN_H__

//#include <d3dx9.h>
#include "Object.h"
#include "Common.h"
#include <vector>

using namespace std;

#define RIPPLE_VERTEX_COUNT 16
#define D3DFVF_RAIN_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct RainVertex{
	D3DXVECTOR3 Pos;
	D3DXCOLOR Color;
};

class RainDrop:public Object
{

	RainVertex dropVertex[2];
	RainVertex rippleVertex[RIPPLE_VERTEX_COUNT];

	float		startHeight;
	D3DXVECTOR3 vPos,aPos;

	D3DXVECTOR3 Dir,BaseDir;
	float		RandDIr;

	float		SIze,MinSIze,MaxSize;

	int			state,time,rippleTime;



public:

	RainDrop(float start_height);
	RainDrop(const RainDrop& rain_drop);
	~RainDrop();

	void Move();
	void Initialize();
	void Appear();

};


class Rain:public ResourceBase{
	vector<RainDrop> rain_drops;
	
public:
	Rain(float start_height,int rain_count);
	~Rain();

	void Add(int add_count);
	void Reduce(int reduce_count);
	void Move();
	void Appear();
};

#endif	/*__RAIN_H__*/