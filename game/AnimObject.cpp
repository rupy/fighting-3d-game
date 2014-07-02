#include "AnimObject.h"
#include "Error.h"

AnimObject::AnimObject(vector<Effect*>& eff,
					   AnimMesh* type,int animIndex,
					   D3DXVECTOR3 position,
					   float Y,float R,float P)
					   :MeshObjectBase(eff,position,Y,R,P),
					   MeshType(type),
					   animIndex(animIndex),
					   animSpeed(1.0f),
					   animStop(false),
					   animChange(true),
					   damage(true)
{
	if(MeshType){
		oldAnim= MeshType->GetAnimation(0);
	}
}
AnimObject::~AnimObject(){
	if(myMesh){
		SAFE_DELETE(MeshType);
	}
}

void AnimObject::Animation(){

	// �A�j���[�V�����̐i�s
	ModelAnimation* new_anim=MeshType->GetAnimation(animIndex);
	if (oldAnim!=new_anim) {

		// ���
		MeshType->SaveSmoother();
		MeshType->SmootherWeight=1;

		// �A�j���[�V�����؂�ւ�
		MeshType->ResetAnimationWeight();
		new_anim->Weight=1;
		new_anim->Time=0;

	} else if (oldAnim->IsOver()) {
//		D3DXMATRIXA16& m=g_Unit.CenterFrame->CombinedMatrix;
		oldAnim->Time=0;
		animIndex=1;
		OkAnimChange();
		OkDamage();
//		animIndex=0;
//		AnimStop();
//		MeshType->ResetAnimationWeight();
//		MeshType->SaveSmoother();
//		MeshType->SmootherWeight=1;

	} else {
		MeshType->SmootherWeight-=0.2f;
		if (MeshType->SmootherWeight<0) MeshType->SmootherWeight=0;
		oldAnim->Time+=animSpeed;
	}

	oldAnim = new_anim;
}


void AnimObject::Shadow(){
	

	//Pos.y++;
	D3DXMATRIXA16 matWorld = MatrixSetting();
	//Pos.y--;
	MeshType->AnimateFrame(&matWorld);
	MeshType->AnimateVertex();

	D3DXMatrixIdentity(&matWorld);
	if(FAILED(rEff[1]->SetMatrix(&matWorld,"g_world"))){
		return;
	}

	if(FAILED(rEff[1]->SetTechnique())){
		return;
	}

	rEff[1]->pFX->CommitChanges();

	// �o�b�t�@�AFVF
	//���_�o�b�t�@�̓��e�������_�����O�p�C�v���C���ɓn��
	GetDevice()->SetStreamSource(0, MeshType->AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	//�C���f�b�N�X�o�b�t�@���w��i���_�����L����O�p�`�̖��ʂ��Ȃ��j
	GetDevice()->SetIndices(MeshType->original->IndexBuffer);
	//FVF�̌`�����w��
	GetDevice()->SetFVF(ANIMATED_FVF);

	UINT numPass;

	if(FAILED(rEff[1]->pFX->Begin(&numPass, 0))){
		return;
	}

	for (size_t i=0; i<MeshType->original->Subset.size(); i++) {

		ModelSubset* subset = MeshType->original->Subset[i];

		rEff[1]->pFX->BeginPass(0);
		// �`��
		if(FAILED(GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0, 0, MeshType->original->VertexCount, 
			subset->FaceIndex*3, subset->FaceCount))){
				return ;
		}

		rEff[1]->pFX->EndPass();
	}

	rEff[1]->pFX->End();
	
}

void AnimObject::Appear(){

	//�A�j���[�V�������b�V���͒��_���g�����[���h���W�Ȃ̂Œ��_�ϊ������ʁB
	//AnimateFrame�ɂ͌v�Z�������[���h�s���n�����A
	//�p�C�v���C���ɂ͒P�ʍs���n���B
	//�t�ɂ��Ă��������\������邪�A�����蔻��Ȃǂŕs���������̂ł��̕��j�ŁB


	D3DXMATRIXA16 matWorld = MatrixSetting();
	MeshType->AnimateFrame(&matWorld);
	MeshType->AnimateVertex();

	if(FAILED(rEff[0]->SetTechnique())){
		return;
	}

	D3DXMatrixIdentity(&matWorld);
	if(FAILED(rEff[0]->SetMatrix(&matWorld,"g_world"))){
		return;
	}

	if(FAILED(rEff[0]->SetMatrix(&matWorld,"g_world_inv"))){
		return;
	}

	if(FAILED(rEff[0]->SetBool(&use_shadow,"g_use_shadow"))){
		return;
	}

	UINT numPass;

	if(FAILED(rEff[0]->pFX->Begin(&numPass, 0))){
		return;
	}
	
	// �o�b�t�@�AFVF

	//���_�o�b�t�@�̓��e�������_�����O�p�C�v���C���ɓn��
	GetDevice()->SetStreamSource(0, MeshType->AnimatedVertexBuffer, 0, sizeof(ANIMATED_VERTEX));
	//�C���f�b�N�X�o�b�t�@���w��i���_�����L����O�p�`�̖��ʂ��Ȃ��j
	GetDevice()->SetIndices(MeshType->original->IndexBuffer);
	//FVF�̌`�����w��
	GetDevice()->SetFVF(ANIMATED_FVF);

	for (size_t i=0; i<MeshType->original->Subset.size(); i++) {

		ModelSubset* subset = MeshType->original->Subset[i];

		// �F�A�e�N�X�`��
		DWORD mi=subset->MaterialIndex;

		if(MeshType->Material[mi]->Texture != 0){
			rEff[0]->pFX->BeginPass(0);
		}
		else{
			rEff[0]->pFX->BeginPass(1);
		}

		// �V���h�E�}�b�v�փA�N�Z�X����Ƃ��̃I�t�Z�b�g�ݒ�
		D3DXVECTOR4	tmpOffset;
		tmpOffset.x = 0.5f/640.0f;
		tmpOffset.y = 0.5f/480.0f;
		if(FAILED(rEff[0]->SetVector(&tmpOffset,"g_offset"))){
			return;
		}

		D3DXVECTOR4		tmpColor;
		tmpColor=MeshType->Material[mi]->Color;


		if(FAILED(rEff[0]->SetTexture(MeshType->Material[mi]->Texture,"g_texture"))){
			return;
		}

		if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color0"))){
			return;
		}

		tmpColor.x = 0.2f;
		tmpColor.y = 0.2f;
		tmpColor.z = 0.2f;
		tmpColor.w = 1.0f;
		if(FAILED(rEff[0]->SetVector(&tmpColor,"g_color1"))){
			return;
		}

		if(FAILED(rEff[0]->pFX->CommitChanges())){
			return;
		}

		// �`��
		GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0, 0, MeshType->original->VertexCount, 
			subset->FaceIndex*3, subset->FaceCount);

		rEff[0]->pFX->EndPass();
	}

	rEff[0]->pFX->End();
}

D3DXMATRIX AnimObject::GetAttachMatrix(char* framename){

	D3DXMATRIXA16 matWorld = MatrixSetting();
	MeshType->AnimateFrame(&matWorld);
	MeshType->AnimateVertex();
	MeshFrame* frame = MeshType->FindFrame(framename);
	if(frame){
		return MeshType->GetFrame(frame->Index)->CombinedMatrix;
	}else{
		return MeshType->GetFrame(0)->CombinedMatrix;
	}
}

HRESULT AnimObject::LoadMeshFromXFile(char* filename){
	if(!MeshType && !myMesh){
		MeshType = new AnimMesh(GetDevice());
		if(FAILED(MeshType->LoadModelsFromXFile(filename , false))){
			DisplayErrMsg("���f���̓ǂݍ��݂Ɏ��s���܂���");
			return(E_FAIL);
		}
	}else{
		DisplayErrMsg("���łɃ��f�����o�^����Ă���A�j���[�V�������b�V���Ƀ��f����ǂݍ������Ƃ��܂����B");
		return E_FAIL;
	}
	
	oldAnim= MeshType->GetAnimation(0);
	myMesh = true;
	return S_OK;
}
int AnimObject::GetAnimIndex(){
	return animIndex;
}
bool AnimObject::GetAnimStop(){
	return animStop;
}
float AnimObject::GetAnimSpeed(){
	return animSpeed;
}
bool AnimObject::GetAnimChange(){
	return animChange;
}
bool AnimObject::GetDamage(){
	return damage; 
}
void AnimObject::SetAnimIndex(int index){
	animIndex = index;
}
void AnimObject::SetAnimSpeed(float speed){
	animSpeed = speed;
}
void AnimObject::SetAnimChange(bool change){
	animChange = change;
}
void AnimObject::SetDamage(bool damage){
	this->damage = damage;
}

void AnimObject::AnimStop(){

	animStop = true;
	oldAnim->Time=0;
}
void AnimObject::AnimStart(){

	animStop = false;
	if(!oldAnim->Time){
		oldAnim->Time=0;
	}
}
AnimMesh* AnimObject::GetMesh(){
	return MeshType;
}
void AnimObject::SetMesh(AnimMesh* mesh,bool give_clone){
	myMesh = give_clone;
	MeshType = mesh;
	oldAnim= MeshType->GetAnimation(0);
}
void AnimObject::NoAnimChange(){
	animChange = false;
}
void AnimObject::OkAnimChange(){
	animChange = true;
}
void AnimObject::NoDamage(){
	damage = false;
}
void AnimObject::OkDamage(){
	damage = true;
}