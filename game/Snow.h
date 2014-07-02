#ifndef __SNOW_H__
#define __SNOW_H__

#include "Object.h"
#include "Common.h"
#include <vector>
#include <algorithm>

using namespace std;

#define D3DFVF_SNOW_VERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0))
#define SNOW_SPEED -0.03f
#define SNOWBALL_TEX_FILE "picture/snow.png"

struct SnowVertex{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
};
class SnowBall:public Object
{
	float	startHeight;
public:
	float	dirCam;

	SnowBall(float start_height);
	//SnowBall(const SnowBall& snow_ball);
	void Initialize();
	void Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	
	void Appear();

};
bool comp(const SnowBall& a,const SnowBall& b);

class Snow:public ResourceBase{
	vector<SnowBall>	snow_ball;
	LPDIRECT3DTEXTURE9	snowPic;
	SnowVertex			ballVertex[4];
	
public:
	Snow(float start_height,int snow_count);
	~Snow();

	void Add(int add_count);
	void Reduce(int reduce_count);
	void Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	void Appear();
	HRESULT SetupTextures();
};
#endif	/*__SNOW_H__*/