#include "BeachScene.h"
#include "HitTest.h"


BeachScene::BeachScene(LPDIRECT3DDEVICE9 device,LPD3DXSPRITE sprite,Sound* sound,SaveManager* sm)
:SceneBase(device,sprite,sound,sm),
//�}�b�v�̏�����
mapObj(eff),
sea(eff),
sky(eff),
//�v���C���[�L�����̏�����
hitoObj(eff,NULL,1,D3DXVECTOR3(10.0f,100.0f,-5.0f)),
hitoPlayer(camera[0],&hitoObj),
carObj(eff,NULL,D3DXVECTOR3(0.0f,100.0f,-5.0f)),
carPlayer(camera[0],&carObj),
camOp(camera[0],&hitoObj),
sword(eff,NULL),command(true),
//�G�L�����̏�����
hitoObj2(eff,NULL,1,D3DXVECTOR3(0.0f,100.0f,5.0f)),
hitoCPU(&hitoObj2),
//�G�t�F�N�g�̏�����
bill(eff,NULL,D3DXVECTOR3(0,5.0f,0)),
rainObject(50,true),
heal(NULL),
snow(10.0f,100),
rain(10.0f,100),
//����e�X�g�̏�����
arrowObj(eff),
shadowtest(300,300),
lines(),
//�\�����̏�����
status_menu(&sm->state.party),
HPMeter(D3DXVECTOR3(20.0f,120.0f,0)),
MPMeter(D3DXVECTOR3(20.0f,100.0f,0))
{
}
BeachScene::~BeachScene(){
	
	pSound->StopASound(&SE[0]);
	SAFE_DELETE(heal);
}
HRESULT BeachScene::Initialize(){

	//���C�g�̏�����
	light = new Light(eff,D3DXVECTOR3(0.0f,0.0f,0.0f));
//	camera.push_back(new Camera(eff,
//						D3DXVECTOR3( 15.0f, 15.0f, -15.0f), 
//						D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 
//						D3DXVECTOR3( 0.0f, 1.0f, 0.0f)));

	//�V�F�[�_�[�̏�����
	if(FAILED(SetupShader())){
		return E_FAIL;
	}
	//�}�b�v�̏�����
	if(FAILED(SetupMap())){
		return E_FAIL;
	}
	//�v���C���[�L������������
	if(FAILED(SetupPlayer())){
		return E_FAIL;
	}
	//�G�L������������
	if(FAILED(SetupEnemy())){
		return E_FAIL;
	}
	//�G�t�F�N�g��������
	if(FAILED(SetupEffect())){
		return E_FAIL;
	}
	//����e�X�g��������
	if(FAILED(SetupTest())){
		return E_FAIL;
	}
	//����e�X�g��������
	if(FAILED(SetupDisplay())){
		return E_FAIL;
	}
	//����e�X�g��������
	if(FAILED(SetupSound())){
		return E_FAIL;
	}

	//�����蔻��̒�`
	HitDefine();

	//���̑��̏���
	if(FAILED(Prepare())){
		return E_FAIL;
	}
	return(S_OK);
}
//=============================================================
// �}�b�v��������
//=============================================================
HRESULT BeachScene::SetupMap(){

	//�}�b�v
	if(pSM->state.file.empty()){
		pSM->state.file = "models/map/moeghi.x";
	}
	mapObj.LoadMeshFromXFile(pSM->state.file.c_str(),"models/map/");//mapObj.Pos.z -= 5.0f;
	//mapObj.Roll += D3DX_PI/10.0f;
	sword.LoadMeshFromXFile("models/sword.x");

	sky.LoadMeshFromXFile("models/sky.x");
	sky.use_shadow = false;
	sky.Scale *= 3.0f;
	return(S_OK);
}
//=============================================================
// �v���C���[�L������������
//=============================================================
HRESULT BeachScene::SetupPlayer(){

	//�l��
	hitoObj.LoadMeshFromXFile("models/hito6.x");

	//��
	carObj.LoadMeshFromXFile("models/car.x");

	// ����
	mapObj.Landing(&hitoObj,0.9f);
	mapObj.Landing(&carObj,1.5f);

	return(S_OK);
}
//=============================================================
// �G�L������������
//=============================================================
HRESULT BeachScene::SetupEnemy(){

	hitoObj2.SetMesh(hitoObj.GetMesh()->Clone());
	//�ڕW�ݒ�
	hitoCPU.SetTargetObject(&hitoObj);

	//����
		mapObj.Landing(&hitoObj2,0.9f);

	return(S_OK);
}
//=============================================================
// �G�t�F�N�g��������
//=============================================================
HRESULT BeachScene::SetupEffect(){

	bill.SetupTexture("picture/star.png");
	bill.parentObj = &hitoObj;


	return(S_OK);
}
//=============================================================
// ����e�X�g��������
//=============================================================
HRESULT BeachScene::SetupTest(){

	//�x�N�g���̃e�X�g
	arrowObj.parentObj = &hitoObj;

	return(S_OK);
}
//=============================================================
// �\������������
//=============================================================
HRESULT BeachScene::SetupDisplay(){

	return(S_OK);
}
//=============================================================
//�G�t�F�N�g�̓ǂݍ���
//=============================================================
HRESULT BeachScene::SetupShader(){

	eff.push_back(new Effect);
	EffectInfo info;
	info.fileName = "shadow.fx";
	info.techName = "BasicTech";
	info.valueName.push_back("g_world");
	info.valueName.push_back("g_world_inv");
	info.valueName.push_back("g_view");
	info.valueName.push_back("g_proj");
	info.valueName.push_back("g_color0");
	info.valueName.push_back("g_color1");
	info.valueName.push_back("g_light_dir");
	info.valueName.push_back("g_texture");
	info.valueName.push_back("g_shadowbuf");
	info.valueName.push_back("g_offset");
	info.valueName.push_back("g_vlp");
	info.valueName.push_back("g_use_shadow");
	info.valueName.push_back("g_fog");
	//�G�t�F�N�g�̓ǂݍ���
	if(FAILED(eff.back()->InitEffect(&info))){
		return E_FAIL;
	}
	eff.push_back(new Effect);
	EffectInfo info_s;
	info_s.fileName = "shadowmap.fx";
	info_s.techName = "BasicTech";
	info_s.valueName.push_back("g_world");
	info_s.valueName.push_back("g_vlp");
	//�G�t�F�N�g�̓ǂݍ���
	if(FAILED(eff.back()->InitEffect(&info_s))){
		return E_FAIL;
	}
	target.push_back(new RenderTarget(pDevice));

	if(FAILED(target.back()->InitSurface(2560,1880))){
		return E_FAIL;
	}
	

	//�t�H�O�̐ݒ�
	D3DXVECTOR4 fog = D3DXVECTOR4(1000.0f/(1000.0f-1.0f),-1.0f/(1000.0f-1.0f),0,0);// (Far/(Far-Near), -1/(Far-Near))
	if(FAILED(eff[0]->SetVector(&fog,"g_fog"))){
		return E_FAIL;
	}

	return S_OK;
}
//=============================================================
//�T�E���h�̓ǂݍ���
//=============================================================
HRESULT BeachScene::SetupSound(){
	
	if(FAILED(pSound->LoadFromWaveFile("sound/wave.wav",&SE[0],DSBCAPS_CTRLVOLUME|DSBCAPS_GLOBALFOCUS,GUID_NULL,4))){
		MessageBox( NULL, "�T�E���h�̓ǂݍ��݂Ɏ��s���܂���", WINDOW_TITLE, MB_OK|MB_ICONHAND );	
		return(E_FAIL);
	}
	pSound->PlayASound(&SE[0],0,DSBPLAY_LOOPING,TRUE);
	return S_OK;
}
//=============================================================
//�����蔻��̒�`
//=============================================================
void BeachScene::HitDefine(){
		//�U������
	Hit HitAttack[]={
		{"LeftHand",0.8f},
		{"RightHand",1.0f},
		{"LeftFoot",0.8f},
		{"RightFoot",0.8f},
		{"Head",0.8f}
	};
	hitoObj.GetMesh()->SetHitAttack(HitAttack,sizeof(HitAttack)/sizeof(Hit));

	//�h�䔻��
	Hit HitDefense[]={
		{"Parent",0.3f},
		{"Waist",0.3f},
		{"Stomach",0.3f},
		{"Breast",0.3f},
		{"LeftShoulder",0.3f},
		{"LeftUpperArm",0.3f},
		{"LeftLowerArm",0.3f},
		{"LeftHand",0.3f},
		{"RightShoulder",0.3f},
		{"RightUpperArm",0.3f},
		{"RightLowerArm",0.3f},
		{"RightHand",0.3f},
		{"Neck_Parent_",0.3f},
		{"Neck",0.3f},
		{"Head_Parent_",0.3f},
		{"Head",0.3f},
		{"LeftUpperKnee_Parent_",0.3f},
		{"LeftUpperKnee",0.3f},
		{"LeftLowerKnee",0.3f},
		{"LeftFoot_Parent_",0.3f},
		{"LeftFoot",0.3f},
		{"RightUpperKnee_Parent_",0.3f},
		{"RightUpperKnee",0.3f},
		{"RightLowetKnee_Parent_",0.3f},
		{"RightLowetKnee",0.3f},
		{"RightFoot_Parent_",0.3f},
		{"RightFoot",0.3f}
	};
	hitoObj.GetMesh()->SetHitDefense(HitDefense,sizeof(HitDefense)/sizeof(Hit));
	hitoObj.GetMesh()->CalcBoundingSphere();
}
//=============================================================
//����
//=============================================================
HRESULT BeachScene::Prepare(){

	sceneMode = FIELD;
	return S_OK;
}
//=============================================================
//�ړ�
//=============================================================
SceneBase* BeachScene::Move(KeyMessage* keyMessage){

	switch(sceneMode){
		case FIELD:
			FieldMode(keyMessage);
			break;
		case STATUS:
			StatusMode(keyMessage);
			break;
		case TALK:
			TalkMode(keyMessage);
			break;
		case GO_TITLE:
			return new StartScene(pDevice,pSprite,pSound,pSM);
			break;
		default:
			break;
	}

	return 0;
}

//=============================================================
//�t�B�[���h
//=============================================================
void BeachScene::FieldMode(KeyMessage* keyMessage){

	//���C�g�̈ړ�
	LightMove();
	//�v���C���[�̈ړ�
	PlayerMove(keyMessage);
	//�G�̈ړ�
	EnemyMove();
	//�G�t�F�N�g�̓���
	EffectMove();
	//�}�b�v�̓���
	MapMove();
	//��
	MapWall();
	//�d��
	MapGravity();
	//�ŏI�I�ȓ����蔻��
	CheckHit();
	//���ׂĂ̕��̂̈ړ�
	AllMove();
	//�e�X�g�̓���
	TestMove();
	//�\�����̓���
	DisplayMove();
	//�A�j���[�V����
	Animation();
	
	if(keyMessage->cmd & NEW_SELECT){
		msg.SelectMsg("start");
		sceneMode = TALK;
		command = false;
	}
}
//=============================================================
//�X�e�[�^�X
//=============================================================
void BeachScene::StatusMode(KeyMessage* keyMessage){

	if(keyMessage->cmd & NEW_START){
		sceneMode = FIELD;
	}
}

//=============================================================
//��b
//=============================================================
void BeachScene::TalkMode(KeyMessage* keyMessage){
	
	EffectMove();
	Animation();
	msg.Move(keyMessage);
	if(keyMessage->cmd & NEW_A){
		int next;
		if(!msg.NextMsg(&next)){
			switch(next){
				case -1:
					sceneMode = GO_TITLE;
					break;
				case 0:
					sceneMode = FIELD;
					break;
				case 1:
					sceneMode = FIELD;
					break;
			}
		}
	}
}

//=============================================================
//�v���C���[�̓���
//=============================================================
void BeachScene::PlayerMove(KeyMessage* keyMessage){
	//�ړ�
	hitoPlayer.Move(keyMessage);
//	carPlayer.Move(keyMessage);
	camOp.Move(keyMessage);
	if(keyMessage->cmd & NEW_Y){
		if(!heal&&pSM->state.party.member1.UseMP(3)){
			pSM->state.party.member1.AllHealHP();
			heal = new Magic(eff,100);
			heal->SetParent(&hitoObj);
		}
	}/*
	if(keyMessage->cmd & NEW_START){
		status_menu.SetData();
		sceneMode = STATUS;
	}*/
}
//=============================================================
//���C�g�̓���
//=============================================================
void BeachScene::LightMove(){

	//���̈ړ�
	light->Pos = hitoObj.Pos;
	DWORD time = timeGetTime();
	light->Pos.x += -20.0f;
	light->Pos.y += 15.0f; 
	light->Pos.z += -20.0f;
	light->lookatPt = hitoObj.Pos;
}
//=============================================================
//�G�̓���
//=============================================================
void BeachScene::EnemyMove(){
	//�ړ�
	hitoCPU.Move();

}
//=============================================================
//�G�t�F�N�g�̓���
//=============================================================
void BeachScene::EffectMove(){
	//�t��
	rainObject.Pos = hitoObj2.Pos;
	rainObject.Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);
	bill.LookAtMe(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);

	rain.Move();
	snow.Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt);

	if(heal){
		if(heal->Move(&hitoPlayer.cam->lookatPt,&hitoPlayer.cam->eyePt)){
			SAFE_DELETE(heal);
		}
	}
}
//=============================================================
//�e�X�g�̓���
//=============================================================
void BeachScene::TestMove(){

	//arrowObj.Pos.z=-1.0f;
	sword.SetParentMatrix(hitoObj.GetAttachMatrix("RightHand"));
	shadowtest.pTex = target.back()->GetTexture();
	VectorTest::SetDir(hitoObj.Velo);
	lines.Pos = hitoObj.Pos;
}
//=============================================================
//�}�b�v�̓���
//=============================================================
void BeachScene::MapMove(){
	sea.Move();
	sky.Pos = hitoObj.Pos;
	sky.Pos.y =-100.0f;
	//mapObj.Roll += D3DX_PI/10000.0f;
}
//=============================================================
//�}�b�v�̏d��
//=============================================================
void BeachScene::MapGravity(){
	hitoPlayer.nowJump = !mapObj.LandHit(&hitoObj,!hitoPlayer.nowJump,0.9f);
	mapObj.LandHit(&hitoObj2,false,0.9f);
	//mapObj.Gravity(&carObj,false,1.3f);
	//mapObj.Gravity(&hitoObj2.Pos,true,0.9f);
	//mapObj.Gravity(&hitoPlayer.cam->eyePt,false,0.9f);
}
//=============================================================
//�}�b�v�Ȃǂ̕ǔ���
//=============================================================
void BeachScene::MapWall(){
	mapObj.WallHit(&hitoObj,10.0f,0.0f);
	mapObj.WallHit(&hitoObj2,7.0f,0.5f);
}
//=============================================================
//�����蔻��ɂ��_���[�W�Ȃ�
//=============================================================
void BeachScene::CheckHit(){
	
	//���̍U��
	if(hitoObj.GetAnimIndex() == 8 && HitTest(&hitoObj,1,&hitoObj2) && hitoObj2.GetDamage()){
		hitoCPU.HP--;
		hitoObj2.NoDamage();
		hitoObj2.SetAnimIndex(7);
//		hitoObj2.AnimStart();
		hitoObj2.SetAnimSpeed(1);
		hitoObj2.NoAnimChange();
	}

	//�G
	//�p���`�U��
	if(hitoObj2.GetAnimIndex() == 3 && HitTest(&hitoObj2,0,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(1);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	//�L�b�N�U��
	if(hitoObj2.GetAnimIndex() == 5 && HitTest(&hitoObj2,2,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(1);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	//���˂��U��
	if(hitoObj2.GetAnimIndex() == 6 && HitTest(&hitoObj2,4,&hitoObj) && hitoObj.GetDamage()){
		pSM->state.party.member1.DamageHP(2);
		hitoObj.NoDamage();
		hitoObj.SetAnimIndex(7);
//		hitoObj.AnimStart();
		hitoObj.SetAnimSpeed(1);
		hitoObj.NoAnimChange();
	}
	if(hitoCPU.HP == 0){
		hitoObj2.SetAnimIndex(9);
		hitoObj2.AnimStop();
	}
	if(pSM->state.party.member1.GetHP() == 0){
		hitoObj.SetAnimIndex(9);
		hitoObj.AnimStop();
	}



}
void BeachScene::AllMove(){
	if(command){
		hitoObj.Move();
		hitoObj2.Move();
	}else{
		//�R�}���h�����͈�x����ł������ɖ߂�
		//��ʐ؂�ւ��̎��ɓ����ɃW�����v�����肷��̂�h���ړI
		command = true;
	}
}
//=============================================================
//�\�����̓���
//=============================================================
void BeachScene::DisplayMove(){

		HPMeter.SetMeter(pSM->state.party.member1.GetHP(),
		pSM->state.party.member1.GetMaxHP());
	MPMeter.SetMeter(pSM->state.party.member1.GetMP(),
		pSM->state.party.member1.GetMaxMP());
}
//=============================================================
//�\�����̓���
//=============================================================
void BeachScene::Animation(){

	hitoObj.Animation();
	hitoObj2.Animation();
}
//=============================================================
//�`��
//=============================================================
void BeachScene::Draw(){
	//cam->CameraSetting();

	DrawShadow();

	DrawObject();
}

//=============================================================
//�e�̕`��
//=============================================================
void BeachScene::DrawShadow(){


	light->LightSetting();
	//�����_�����O�^�[�Q�b�g���e�N�X�`���ɐ؂�ւ���

	target.back()->BeginToggleBackBuffer();

	mapObj.Shadow();
	hitoObj.Shadow();
	//carObj.Shadow();
	hitoObj2.Shadow();
	//arrowObj.Shadow();
	sword.Shadow();
	target.back()->EndToggleBackBuffer();
	// �V���h�E�}�b�v�̐ݒ�
	if(FAILED(eff[0]->SetTexture(target.back()->GetTexture(),"g_shadowbuf"))){
		return ;
	}
	// �V���h�E�}�b�v�փA�N�Z�X����Ƃ��̃I�t�Z�b�g�ݒ�
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f/target.back()->GetTargetWidth();
	tmpOffset.y = 0.5f/target.back()->GetTargetHeight();
	if(FAILED(eff[0]->SetVector(&tmpOffset,"g_offset"))){
		return;
	}
}
//=============================================================
//�I�u�W�F�N�g�̕`��
//=============================================================
void BeachScene::DrawObject(){

	switch(sceneMode){
		case FIELD:
		case TALK:
			mapObj.Appear();
			hitoObj.Appear();
			//carObj.Appear();
			//arrowObj.Appear();
			sword.Appear();
			hitoObj2.Appear();
			//lines.Appear();
			break;
	}

	//==================================
	//�A���t�@�l�����I�u�W�F�N�g��`��
	//==================================
	switch(sceneMode){
		case FIELD:
		case TALK:
			rain.Appear();
			sea.Appear();
			rainObject.Appear();
			
			//==================================
			//�r���{�[�h�̕`��
			//==================================
			SetRenderStateFor2D();
			//bill.Appear();
			if(heal){
				heal->Appear();
			}
			snow.Appear();
			break;
	}
	//==================================
	//�\�����Ȃǂ�2D�����̕`��
	//==================================
	ClearFor2DDraw();
	switch(sceneMode){
		case FIELD:
			//shadowtest.Appear();
			HPMeter.Appear();
			MPMeter.Appear();
			break;
		case STATUS:
			status_menu.Appear();
			break;
		case TALK:
			msg.Appear();
			break;
	}

}