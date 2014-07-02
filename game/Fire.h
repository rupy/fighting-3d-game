#ifndef __FIRE_H__
#define __FIRE_H__

#include "Object.h"
#include "Common.h"
#include <vector>
#include "DummyObject.h"

using namespace std;

#define D3DFVF_FIRE_VERTEX (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0))
#define FIRE_WIDESPREAD		.5f
#define FIRE_MAX_HEIGHT		20.0f
#define FIRE_MAX_SPEED		0.08f
#define FIRE_MIN_SPEED		0.02f
#define FIRE_MAX_SIZE		1.0f
#define FIRE_MIN_SIZE		0.3f
#define FIRE_COLOR_RED		.9f
#define FIRE_COLOR_BLUE		.3f
#define FIRE_COLOR_GREEN	.3f
#define FIRE_MIN_TIME		10
#define FIRE_MAX_TIME		30
#define FIREPARTICLE_TEX_FILE "picture/Smoke.png"

struct FireVertex{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Tex;
};
class FireParticle:public Object
{
	D3DXVECTOR3	vPos;
	int			time;
	int			endtime;
	float		dirCam;

public:
	FireParticle(DummyObject* parent,bool tail);
	void Initialize();
	void Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	
	void Appear();
};

class Fire:public DummyObject{

	vector<FireParticle>	fire_particle;
	LPDIRECT3DTEXTURE9		firePic;
	FireVertex				particleVertex[4];
	bool					tail;//‚µ‚Á‚Û
public:
	Fire(int fire_count,bool tail, D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0));
	~Fire();

	void Add(int add_count);
	void Reduce(int reduce_count);
	void Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	void Appear();
	HRESULT SetupTextures();
};
#endif	/*__FIRE_H__*/