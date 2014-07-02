#include "Effect.h"

Effect::Effect()
:pFX(NULL)
{
}
Effect::~Effect(){
	SAFE_RELEASE(pFX);
}
HRESULT Effect::InitEffect(const EffectInfo* info){


	this->info = *info;
	//�G�t�F�N�g�t�@�C���̃R���p�C��
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}

	//�n���h���̎擾
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}


	return S_OK;
}

HRESULT Effect::EffectCompile(){

	LPD3DXBUFFER err=NULL;
	if(FAILED(D3DXCreateEffectFromFile(GetDevice(),info.fileName.c_str(),NULL,NULL,D3DXSHADER_DEBUG,NULL,&pFX,&err))){
		// �R���p�C���G���[�̏ꍇ�̓G���[��\�����ďI��
		if (!pFX) {
			MessageBox(
				NULL, 
				err?(LPCTSTR)err->GetBufferPointer():NULL, 
				"�G�t�F�N�g�t�@�C���̓Ǎ��G���[", MB_OK);
		}
		return E_FAIL;
	}
	return S_OK;
}
HRESULT Effect::HandleSetting(){

	// �V�F�[�_�v���O�����փe�N�j�b�N�ւ̃n���h���̎擾
	hTech = pFX->GetTechniqueByName(info.techName.c_str());

	for(size_t i=0; i < info.valueName.size(); ++i){
		hValue.insert( make_pair(info.valueName[i].c_str() ,pFX->GetParameterByName(0,info.valueName[i].c_str() )));
	}

	//������ƃn���h�����擾�ł��Ă��邩�`�F�b�N
	map<string,D3DXHANDLE>::iterator p;
	for(p = hValue.begin(); p != hValue.end();++p){
		if(p->second == NULL){
			return E_FAIL;
		}
	}
	return S_OK;
}
HRESULT Effect::Reset(){

	hValue.clear();

	//�G�t�F�N�g�t�@�C���̍ăR���p�C��
	if(FAILED(EffectCompile())){
		return E_FAIL;
	}
	//�n���h���̍Ď擾
	if(FAILED(HandleSetting())){
		return E_FAIL;
	}
	////�T�[�t�F�C�X���Đ���
	//if(FAILED(ResetSurface())){
	//	return E_FAIL;
	//}
	
	return S_OK;
}

LPD3DXEFFECT Effect::GetEffect(){
	return pFX;
}

HRESULT Effect::SetMatrix(const D3DXMATRIX* mat, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetMatrix( p->second , mat))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetVector(const D3DXVECTOR4* vec, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetVector( p->second , vec))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetBool(const bool* value, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetBool( p->second , *value))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetFloat(const float* value, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetFloat( p->second , *value))){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Effect::SetTexture(const LPDIRECT3DTEXTURE9 tex, char* handle_name){

	map<string,D3DXHANDLE>::iterator p = hValue.find(handle_name);

	if(p == hValue.end()){
		return E_FAIL;
	}

	if(FAILED(pFX->SetTexture(p->second, tex))){
		return E_FAIL;
	}
	return S_OK;
		
}
HRESULT Effect::SetTechnique(){

	if(FAILED(pFX->SetTechnique(hTech))){
		return E_FAIL;
	}
	return S_OK;
}
