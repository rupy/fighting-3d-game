#ifndef __ANIMMESH_H__
#define __ANIMMESH_H__

#include <d3dx9.h>
#include <string>
#include <vector>
#include "MeshBase.h"

using namespace std;


HRESULT GetToken();
HRESULT GetToken(const char* token);
float GetFloatToken();
int GetIntToken();
HRESULT SkipNode();

//============================================================================
// ���_�`��

#define MODEL_BLEND_COUNT 4

struct ORIGINAL_VERTEX {
	D3DXVECTOR3 Pos;
	float Weight[MODEL_BLEND_COUNT];
	unsigned char Index[MODEL_BLEND_COUNT];
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
#define ORIGINAL_FVF (D3DFVF_XYZB5|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)

struct ANIMATED_VERTEX {
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};
#define ANIMATED_FVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


//============================================================================

class MeshFrame {
public:
	D3DXMATRIXA16 TransformMatrix;//���_�̈ʒu�ړ��s��
	D3DXMATRIXA16 OffsetMatrix;//�{�[���̍��W���烍�[�J�����W�ɕϊ�����
	D3DXMATRIXA16 CombinedMatrix;//�����s��i�����蔻��ȂǂɎg���j
	D3DXMATRIXA16 SkinningMatrix;//�X�L�j���O�s��i���f���`��Ɏg�p�j
	D3DXMATRIXA16 SmootherMatrix;//�ŏI�I�ɒ��_���ړ�������A�j���؂�ւ��X���[�Y�ȍs��
	string Name;
	vector<MeshFrame*> Child;
	size_t Index;

	MeshFrame();
	~MeshFrame();
	void ReadFrame(vector<MeshFrame*>* frame);
	void FrameClone(MeshFrame* frame,vector<MeshFrame*>* clone);
	void Animate(D3DXMATRIXA16* parent);
};
class ModelSubset {
public:
	DWORD MaterialIndex;
	DWORD FaceIndex;
	DWORD FaceCount;
};

class ModelMaterial {
public:
	D3DXVECTOR4 Color;
	IDirect3DTexture9* Texture;
	ModelMaterial::ModelMaterial(){
		Color=D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
	}
};

class ModelAnimationKey {
public:
	ModelAnimationKey(){}
	ModelAnimationKey(const ModelAnimationKey&);
	~ModelAnimationKey();
	vector<float> Time;
	vector<D3DXMATRIXA16*> Matrix;
	string FrameName;
	size_t FrameIndex;
};

class ModelAnimation {
public:
	ModelAnimation(){}
	ModelAnimation(const ModelAnimation&);
	~ModelAnimation();
	string Name;
	vector<ModelAnimationKey*> Key;
	float Time;
	float Weight;

	float GetMaxTime() { return Key.front()->Time.back(); }
	bool IsOver() { return Time>=GetMaxTime(); }
};

//�����蔻��̈�
struct Hit{
	char * FrameName;	//�t���[����
	float Radius;		//���a
	int FrameIndex;		//�t���[���ԍ�
};

class AnimMeshOriginal
{
public:
	AnimMeshOriginal();
	~AnimMeshOriginal();

	IDirect3DVertexBuffer9* VertexBuffer;
	DWORD VertexCount;
	IDirect3DIndexBuffer9* IndexBuffer;
	DWORD FaceCount;
	vector<ModelSubset*> Subset;
	
	//�����蔻��
	vector<Hit> HitAttack;
	vector<Hit> HitDefense;
	//���E��
	D3DXVECTOR3	center;
	float		radius;
	//���E�{�b�N�X
	D3DXVECTOR3	min;
	D3DXVECTOR3	max;

	size_t RefCount;
};


class AnimMesh:public MeshBase
{
	IDirect3DDevice9* pDevice;

public:
	AnimMeshOriginal* original;

	IDirect3DVertexBuffer9* AnimatedVertexBuffer;
	vector<ModelMaterial*> Material;
	vector<ModelAnimation*> Animation;
	vector<MeshFrame*> Frame;
	float SmootherWeight;
	AnimMesh(IDirect3DDevice9* device);
	~AnimMesh();

	HRESULT LoadModelsFromXFile(const char* file_name, bool anim_only);
	MeshFrame* FindFrame(const char* name);
	MeshFrame* GetFrame(DWORD index) { return Frame[index]; }
	void AnimateFrame(D3DXMATRIXA16* world);
	void AnimateVertex();

	
	//�����蔻�菈��
	void SetHitAttack(Hit* attack,int size);
	void SetHitDefense(Hit* defense,int size);
	vector<Hit>* getHitAttack();
	vector<Hit>* getHitDefense();
	HRESULT CalcBoundingSphere();
	HRESULT CalcBoundingBox();
	void GetBoundingSphere(D3DXVECTOR3* center,float* radius);
	void GetBoundingBox(D3DXVECTOR3* min,D3DXVECTOR3* max);

	ModelAnimation* FindAnimation(const char* name);
	void AddAnimation(ModelAnimation* anim);
	void ResetAnimationWeight();
	DWORD GetAnimationCount() { return (DWORD)Animation.size(); }
	ModelAnimation* GetAnimation(DWORD index) { return Animation[index]; }
	void SaveSmoother();

	AnimMesh* Clone();

private:
	HRESULT ReadAnimation();
	HRESULT ReadMesh();

};
#endif	/*__ANIMMESH_H__*/