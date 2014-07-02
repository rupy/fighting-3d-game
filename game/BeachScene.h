#ifndef __BEACH_SCENE_H__
#define __BEACH_SCENE_H__

#include "Scenebase.h"
#include "StartScene.h"
#include "Mesh.h"
#include "TerrianObject.h"
#include "AnimMesh.h"
#include "AnimObject.h"
#include "Human.h"
#include "MeshPlayer.h"
#include "MeasureLine.h"
#include "Rain.h"
#include "Fire.h"
#include "Sphere.h"
#include "FlipAnimObject.h"
#include "StatusMenu2.h"
#include "AnimCPU.h"

#include "MySpriteObject.h"
#include "WaterField.h"
#include "StatusMeter.h"
#include "BillboardObject.h"
#include "Magic.h"
#include "CameraOperator.h"
#include "VectorTest.h"
#include "MsgBox.h"
#include "Snow.h"
#include "Rain.h"


class BeachScene :public SceneBase{


	enum SceneMode{
		FIELD,
		TALK,
		STATUS,
		GO_TITLE,
	};


public:
	
	//�R���X�g���N�^
	BeachScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm);
	//�f�X�g���N�^
	~BeachScene();

	//������
	//�Q�[���̎��s
	void Draw();
private:

	//=============================================================
	// ������
	//=============================================================
	HRESULT Initialize();

	HRESULT SetupShader();
	void HitDefine();
	HRESULT Prepare();
	


	//���C�g��J�����Ɋւ��邱��
	//=============================================================
	//HRESULT SetupCamera();
	void LightMove();

	//�}�b�v�Ɋւ��邱��
	//=============================================================
	TerrianObject mapObj;//�}�b�v
	WaterField	sea;//�C
	MeshObject sky;//��
	HRESULT SetupMap();
	void MapMove();
	void MapGravity();
	void MapWall();

	//�v���C���[�Ɋւ��邱��
	//=============================================================
	AnimObject hitoObj;
	Human hitoPlayer;
	MeshObject carObj;
	MeshPlayer carPlayer;
	CameraOperator camOp;
	bool command;
	HRESULT SetupPlayer();//�v���[���[�L������������
	void PlayerMove(KeyMessage* keyMessage);
	AttachObject sword;//�����蔻��̕\��

	//�G�L�����Ɋւ��邱��
	//=============================================================
	AnimObject hitoObj2;
	AnimCPU	hitoCPU;
	HRESULT SetupEnemy();//�G�L�����̏�����
	void EnemyMove();

	//�G�t�F�N�g�Ɋւ��邱��
	//=============================================================
	Fire rainObject;
	BillboardObject bill;
	Magic* heal;
	Snow snow;
	Rain rain;
	HRESULT SetupEffect();
	void EffectMove();

	//����e�X�g�Ɋւ��邱��
	//=============================================================
	MySpriteObject shadowtest;//�V���h�E�}�b�v�̕\��
	MeasureLine lines;//�O���b�h�̕\��
	VectorTest arrowObj;//�i�s�����̕\��
	HRESULT SetupTest();
	void TestMove();

	//�\�����Ɋւ��邱��
	//=============================================================
	StatusMenu status_menu;
	StatusMeter HPMeter;
	StatusMeter MPMeter;
	MsgBox		msg;
	HRESULT SetupDisplay();
	void DisplayMove();

	//�T�E���h�Ɋւ��邱��
	//=============================================================
	SoundData SE[NUM_OF_SOUNDTYPE];// ���ʉ�
	HRESULT SetupSound();

	void CheckHit();
	void Animation();
	void AllMove();


	//=============================================================
	// �Q�[���̎��s
	//=============================================================
	SceneBase* Move(KeyMessage* keyMessage);
	void FieldMode(KeyMessage* keyMessage);
	void StatusMode(KeyMessage* keyMessage);
	void TalkMode(KeyMessage* keyMessage);

	//�`��
	void DrawShadow();
	void DrawObject();

};


#endif	/*__BEACH_SCENE_H__*/