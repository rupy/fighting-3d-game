#ifndef __MAGIC_H__
#define __MAGIC_H__

//回復魔法のエフェクト

#include "PolarCoordinates.h"
#include "DummyObject.h"

#define MAGIC_TEX_FILE "picture/Smoke.png"

struct MagicInfo{
	char* tex_file;
	float rad_min_speed;
	float rad_max_speed;
	float height_min_speed;
	float height_max_speed;
	float theta_min_speed;
	float theta_max_speed;
	float size_min;
	float size_max;
	float start_rad;
	float height_max;
	float height_min;
	float color_red;
	float color_blue;
	float color_green;

	MagicInfo(
	char* tex_file			=	MAGIC_TEX_FILE,
	float rad_min_speed		=	0.01f,
	float rad_max_speed		=	0.03f,
	float height_min_speed	=	0.05f,
	float height_max_speed	=	0.1f,
	float theta_min_speed	=	0.1f,
	float theta_max_speed	=	0.3f,
	float size_min			=	0.3f,
	float size_max			=	0.7f,
	float start_rad			=	1.0f,
	float height_max		=	5.0f,
	float height_min		=	-1.0f,
	float color_red			=	0.0f,
	float color_blue		=	0.5f,
	float color_green		=	0.9f
	)
		:tex_file(tex_file),
		rad_min_speed(rad_min_speed),
		rad_max_speed(rad_max_speed),
		height_min_speed(height_min_speed),
		height_max_speed(height_max_speed),
		theta_min_speed(theta_min_speed),
		theta_max_speed(theta_max_speed),
		size_min(size_min),
		size_max(size_max),
		start_rad(start_rad),
		height_max(height_max),
		height_min(height_min),
		color_red(color_red),
		color_blue(color_blue),
		color_green(color_green)
	{}
};

enum MagicMode{
	MAGIC_HEAL,//回復
	MAGIC_IN,//回復
};
class Magic:public Object{
public:
	MagicInfo info;
	int count;
	int mode;
	int time;
	int endtime;
	LPDIRECT3DTEXTURE9 pTex;
	vector<PolarCoordinates*> particle;
	Magic(vector<Effect*>& eff,
		int time);
	~Magic();

	bool Move(D3DXVECTOR3* look_pos,D3DXVECTOR3* cam_pos);
	void Initialize();
	void Initialize(int idx);
	void Appear();
};
#endif	/*__MAGIC_H__*/