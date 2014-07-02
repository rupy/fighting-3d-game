#include "AnimMesh.h"
#include "Common.h"

static char* Pointer;
static char Token[1024];

MeshFrame::MeshFrame(){}
MeshFrame::~MeshFrame(){
}

void MeshFrame::ReadFrame(vector<MeshFrame*>* frame)
{
	// �z��ɒǉ�
	Index=frame->size();
	frame->push_back(this);

	// �s��̏�����
	D3DXMatrixIdentity(&TransformMatrix);
	D3DXMatrixIdentity(&OffsetMatrix);
	D3DXMatrixIdentity(&CombinedMatrix);
	D3DXMatrixIdentity(&SkinningMatrix);
	D3DXMatrixIdentity(&SmootherMatrix);
	
	// ���O
	GetToken();
	Name=Token;
	
	GetToken("{");
	
	while (*Pointer!='\0') {
		GetToken();
		if (strchr(Token, '}')) break;
		
		// �q�t���[��
		if (strcmp(Token, "Frame")==0) {
			Child.push_back(new MeshFrame());
			Child.back()->ReadFrame(frame);
		} else
		
		// �ό`�s��
		if (strcmp(Token, "FrameTransformMatrix")==0) {
			GetToken("{");
			float f[16];
			for (int i=0; i<16; i++) f[i]=GetFloatToken();
			TransformMatrix=D3DXMATRIXA16(f);
			GetToken("}");
		} else
		
		// ���̑��̃m�[�h
		{
			SkipNode();
		}
	}
}

//2�̐��`���X�g���󂯎���Ă���
void MeshFrame::FrameClone(MeshFrame* frame,vector<MeshFrame*>* clone){

	// �z��ɒǉ�
	this->Index=clone->size();
	clone->push_back(this);

	// �s��
	this->TransformMatrix	=	frame->TransformMatrix;
	this->OffsetMatrix		=	frame->OffsetMatrix;
	this->CombinedMatrix	=	frame->CombinedMatrix;
	this->SkinningMatrix	=	frame->SkinningMatrix;
	this->SmootherMatrix	=	frame->SmootherMatrix;
	this->Name = frame->Name;

	// �q�t���[��
	for(size_t i=0;i<frame->Child.size();i++){
		Child.push_back(new MeshFrame());
		Child.back()->FrameClone(frame->Child[i],clone);
	}

}

static vector<IDirect3DTexture9*> Texture;
static vector<char*> TextureName;

static IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name) {
	for (size_t i=0; i<TextureName.size(); i++) {
		if (strcmp(TextureName[i], file_name)==0) return Texture[i];
	}
//	string s=GetExePath()+"models\\"+file_name;
	char s[50];
	sprintf(s,"models\\%s",file_name);
	IDirect3DTexture9* texture=NULL;
	D3DXCreateTextureFromFileEx(
		device, s,
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		NULL, NULL, &texture);
	Texture.push_back(texture);
	char* t=new char[strlen(file_name)+1];
	strcpy(t, file_name);
	TextureName.push_back(t);
	return texture;
}
AnimMesh::AnimMesh(IDirect3DDevice9* device)
:pDevice(device), original(new AnimMeshOriginal), AnimatedVertexBuffer(NULL), SmootherWeight(0){
}
AnimMeshOriginal::AnimMeshOriginal()
		: VertexBuffer(NULL), VertexCount(0), 
		  IndexBuffer(NULL), FaceCount(0), RefCount(0) {}
AnimMeshOriginal::~AnimMeshOriginal(){
	// Subset
	for (size_t i = 0; i < Subset.size(); ++i) {
		SAFE_DELETE(Subset[i]);
	}
	Subset.clear();
}
AnimMesh::~AnimMesh(){
	// Material
	for (size_t i = 0; i < Material.size(); ++i) {
		SAFE_DELETE(Material[i]);
	}
	Material.clear();

	// Animation
	for (size_t i = 0; i < Animation.size(); ++i) {
		SAFE_DELETE(Animation[i]);
	}
	Animation.clear();

	for(size_t i = 0;i < Frame.size();++i){
		SAFE_DELETE(Frame[i]);
	}

	// ���L�����̉��
	if (original->RefCount == 0) {
		delete original;
		original = NULL;
	} else {
		original->RefCount--;
	}
}
ModelAnimationKey::ModelAnimationKey(const ModelAnimationKey & org)
:FrameName(org.FrameName),FrameIndex(org.FrameIndex)
{
	for(size_t i=0;i<org.Time.size();++i){
		Time.push_back(org.Time[i]);
	}
	for(size_t i=0;i<org.Matrix.size();++i){
		Matrix.push_back( new D3DXMATRIXA16(*org.Matrix[i]));
	}
}
ModelAnimationKey::~ModelAnimationKey(){
	for(size_t i=0;i < Matrix.size();++i){
		SAFE_DELETE(Matrix[i]);
	}
}
ModelAnimation::ModelAnimation(const ModelAnimation& org)
:Time(org.Time),Weight(org.Weight),Name(org.Name)
{
	for(size_t i=0;i<org.Key.size();++i){
		this->Key.push_back(new ModelAnimationKey(*org.Key[i]));
	}
}
ModelAnimation::~ModelAnimation(){
	for(size_t i=0;i<Key.size();++i){
		SAFE_DELETE(Key[i]);
	}
}

HRESULT AnimMesh::LoadModelsFromXFile(const char* file_name, bool anim_only){

	// �������t�@�C���̓ǂݍ��݁�����
	string s=file_name;

	HANDLE file=CreateFile(
		s.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (file==INVALID_HANDLE_VALUE) {
		char s[1024];
		sprintf_s(s, sizeof(s), "�t�@�C��%s�̓ǂݍ��݂Ɏ��s", file_name);
		
		MessageBox(NULL, s, "���f���̓ǂݍ��݃G���[", MB_OK);
		return E_FAIL;
	}

	DWORD size=GetFileSize(file, NULL);
	char* buffer=new char[size+1];
	buffer[size]='\0';

	DWORD read_size;
	ReadFile(file, buffer, size, &read_size, NULL);

	CloseHandle(file);
	if (read_size!=size) return E_FAIL;

	// �������t�@�C���̓ǂݍ��݁�����
	
	Pointer=buffer;
	ReadAnimation();

	Pointer=buffer;
	if(!anim_only)
		ReadMesh();

	
	// ��ЂÂ�
	delete[] buffer;

	return S_OK;
}

HRESULT AnimMesh::ReadAnimation(){
	while (*Pointer!='\0') {
		GetToken();

		// �e���v���[�g
		if (strcmp(Token, "template")==0) {
			SkipNode();
		} else
	
		// �t���[��
		if (strcmp(Token, "Frame")==0) {
			if (!Frame.empty()) {
				MessageBox(NULL, "�t�@�C�����ɕ����̃��[�g�t���[��", "���f���̓ǂݍ��݃G���[", MB_OK);
				return E_FAIL;
			}
			MeshFrame* tmpFrame =new MeshFrame();
			tmpFrame->ReadFrame(&Frame);
		}

		// �A�j���[�V����
		if (strcmp(Token, "AnimationSet")==0) {
			ModelAnimation* anim=new ModelAnimation();
			Animation.push_back(anim);
			anim->Time=0;
			anim->Weight=0;
			
			GetToken();
			anim->Name = Token;
			
			GetToken("{");
			;

			while (*Pointer!='\0') {
				GetToken();
				if (strchr(Token, '}')) break;
				
				if (strcmp(Token, "Animation")==0) {
					ModelAnimationKey* key=new ModelAnimationKey();
					anim->Key.push_back(key);
					
//					GetToken();
					GetToken("{");

					//GetToken("{");
					//GetToken();
					//key->FrameName=new char[strlen(Token)+1];
					//strcpy(key->FrameName, Token);
					//key->FrameIndex=FindFrame(key->FrameName)->Index;
					//GetToken("}");


					GetToken("AnimationKey");
					GetToken("{");

					GetToken();

					if(strcmp(Token, "4")==0){//�s��̎�
						DWORD key_count=GetIntToken();//�L�[��
						for (DWORD i=0; i<key_count; i++) {
							key->Time.push_back((float)GetIntToken());//key����
							GetToken("16");
							float f[16];
							for (DWORD j=0; j<16; j++) f[j]=GetFloatToken();
							key->Matrix.push_back(new D3DXMATRIXA16(f));
						}
					}
					else{//key_count�͂��ׂĈꏏ���ƃJ�E���g���Ă���
						if(strcmp(Token, "0")==0){
							D3DXQUATERNION* rot;
							D3DXVECTOR3* scl;
							D3DXVECTOR3* pos;

							DWORD key_count=GetIntToken();//�L�[��
							
							rot = new D3DXQUATERNION[key_count];
							for (DWORD i=0; i<key_count; i++) {//���ׂĂ̎��ԕ���ǂݍ���
//								key->Time.push_back((float)GetIntToken());//key����
								GetIntToken();
								GetToken("4");//��]�͂S
								rot[i].w = GetFloatToken();
								rot[i].x = -GetFloatToken();
								rot[i].y = -GetFloatToken();
								rot[i].z = -GetFloatToken();
							}
							
							GetToken("}");
							GetToken("AnimationKey");
							GetToken("{");
							GetToken("1");
							key_count=GetIntToken();//�L�[��
							
							scl = new D3DXVECTOR3[key_count];
							for (DWORD i=0; i<key_count; i++) {//���ׂĂ̎��ԕ���ǂݍ���
//								key->Time.push_back((float)GetIntToken());//key����
								GetIntToken();
								GetToken("3");//�X�P�[����3
								scl[i].x = GetFloatToken();
								scl[i].y = GetFloatToken();
								scl[i].z = GetFloatToken();
							}

							GetToken("}");
							GetToken("AnimationKey");
							GetToken("{");
							GetToken("2");
							key_count=GetIntToken();//�L�[��
							
							pos = new D3DXVECTOR3[key_count];
							for (DWORD i=0; i<key_count; i++) {//���ׂĂ̎��ԕ���ǂݍ���
								key->Time.push_back((float)GetIntToken());//key����
								GetToken("3");//�ړ���3
								pos[i].x = GetFloatToken();
								pos[i].y = GetFloatToken();
								pos[i].z = GetFloatToken();
							}

							for(DWORD i=0; i<key_count; i++){
								D3DXMATRIXA16 sc;
								D3DXMATRIXA16 tl;
								D3DXMATRIXA16 rt;
								D3DXMATRIXA16* mat = new D3DXMATRIXA16;
								D3DXMATRIXA16 tmp;
								D3DXMatrixIdentity(&sc);
								D3DXMatrixIdentity(&tl);
								D3DXMatrixIdentity(&rt);
								D3DXMatrixIdentity(mat);
								D3DXMatrixIdentity(&tmp);
								D3DXQUATERNION r = rot[i];
								D3DXVECTOR3 s= scl[i];
								D3DXVECTOR3 p= pos[i];
//								D3DXMatrixTransformation(&mat,NULL,NULL,&s,NULL,&r,&p);
								D3DXMatrixRotationQuaternion(&rt,&r);
								D3DXMatrixTranslation(&tl,p.x,p.y,p.z);
								D3DXMatrixScaling(&sc,s.x,s.y,s.z);
								D3DXMatrixMultiply(&tmp,&sc,&rt);
								D3DXMatrixMultiply(mat,&tmp,&tl);

								key->Matrix.push_back(mat);
							}


							delete[] rot;
							delete[] scl;
							delete[] pos;
						}
					}



					GetToken("}");
//
					GetToken("{");
					GetToken();
					key->FrameName=Token;
					key->FrameIndex=FindFrame(key->FrameName.c_str())->Index;
					GetToken("}");
//					
					GetToken("}");
				}
			}
		}
	}

	return S_OK;
}
HRESULT AnimMesh::ReadMesh(){
	while (*Pointer!='\0') {
		GetToken();
			
		// �e���v���[�g
		if (strcmp(Token, "template")==0) {
			SkipNode();
		} else
		
		// ���b�V��
		if (strcmp(Token, "Mesh")==0) {
			if (original->VertexBuffer) {
				MessageBox(NULL, "�t�@�C�����ɕ����̃��b�V��","���f���̓ǂݍ��݃G���[" , MB_OK);
				return E_FAIL;
			}
			GetToken();			
			GetToken("{");

			// ���_
			original->VertexCount=GetIntToken();
			pDevice->CreateVertexBuffer(
				sizeof(ORIGINAL_VERTEX)*original->VertexCount, 0, 
				ORIGINAL_FVF, D3DPOOL_SYSTEMMEM,
				&original->VertexBuffer, NULL);
			pDevice->CreateVertexBuffer(
				sizeof(ANIMATED_VERTEX)*original->VertexCount, 0, 
				ANIMATED_FVF, D3DPOOL_MANAGED,
				&AnimatedVertexBuffer, NULL);
			if (!original->VertexBuffer || !AnimatedVertexBuffer) {
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","���f���̓ǂݍ��݃G���[" , MB_OK);
				return E_FAIL;
			}
			void* vb;
			original->VertexBuffer->Lock(0, 0, &vb, 0);
			ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
			for (DWORD i=0; i<original->VertexCount; i++) {
				v[i].Pos.x=GetFloatToken();
				v[i].Pos.y=GetFloatToken();
				v[i].Pos.z=GetFloatToken();
				for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) v[i].Index[j]=0;
				v[i].Weight[0]=1;
				for (DWORD j=1; j<MODEL_BLEND_COUNT; j++) v[i].Weight[j]=0;
			}
			original->VertexBuffer->Unlock();

			// �C���f�b�N�X
			original->FaceCount=GetIntToken();
			pDevice->CreateIndexBuffer(
				sizeof(WORD)*3*original->FaceCount, 0, 
				D3DFMT_INDEX16, D3DPOOL_MANAGED, 
				&original->IndexBuffer, NULL);
			if (!original->IndexBuffer) {
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s" , MB_OK);
				return E_FAIL;
			}
			void* ib;
			original->IndexBuffer->Lock(0, 0, &ib, 0);
			WORD* index=(WORD*)ib;
			for (DWORD i=0, n=original->FaceCount*3; i<n; i+=3) {
				GetToken("3");
				for (DWORD j=0; j<3; j++) {
					index[i+j]=GetIntToken();
				}
			}
			original->IndexBuffer->Unlock();
		} else
			
		// �@��
		if (strcmp(Token, "MeshNormals")==0) {
			GetToken("{");

			if (GetIntToken()!=original->VertexCount) {
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","���_���Ɩ@�������s��v" , MB_OK);
				return E_FAIL;
			}

			void* vb;
			original->VertexBuffer->Lock(0, 0, &vb, 0);
			ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
			for (DWORD i=0; i<original->VertexCount; i++) {
				v[i].Normal.x=GetFloatToken();
				v[i].Normal.y=GetFloatToken();
				v[i].Normal.z=GetFloatToken();
			}
			original->VertexBuffer->Unlock();

			if (GetIntToken()!=original->FaceCount) {				
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","�ʐ��Ɩ@���C���f�b�N�X�����s��v" , MB_OK);
				return E_FAIL;
			}
			for (DWORD i=0, n=original->FaceCount*4; i<n; i++) GetToken();
		} else
						
		// �e�N�X�`�����W
		if (strcmp(Token, "MeshTextureCoords")==0) {
			GetToken("{");
		
			if (GetIntToken()!=original->VertexCount) {
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","���_���ƃe�N�X�`�����W�����s��v" , MB_OK);
				return E_FAIL;
			}
	
			void* ovb;
			original->VertexBuffer->Lock(0, 0, &ovb, 0);			
			ORIGINAL_VERTEX* ov=(ORIGINAL_VERTEX*)ovb;
			void* avb;
			AnimatedVertexBuffer->Lock(0, 0, &avb, 0);			
			ANIMATED_VERTEX* av=(ANIMATED_VERTEX*)avb;
			for (DWORD i=0; i<original->VertexCount; i++) {
				ov[i].UV.x=av[i].UV.x=GetFloatToken();
				ov[i].UV.y=av[i].UV.y=GetFloatToken();
			}
			original->VertexBuffer->Unlock();
			AnimatedVertexBuffer->Unlock();
		} else
		// �}�e���A�����X�g
		if (strcmp(Token, "MeshMaterialList")==0) {
			GetToken("{");
			DWORD material_count=GetIntToken();
			
			if (GetIntToken()!=original->FaceCount) {
				MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s","�ʐ��ƃ}�e���A�����X�g���̕s��v" , MB_OK);
				return E_FAIL;
			}
			// �T�u�Z�b�g�̍쐬
			ModelSubset* subset=NULL;
			DWORD prev_index=-1;
			for (DWORD i=0; i<original->FaceCount; i++) {
				DWORD index=GetIntToken();
				if (index!=prev_index) {
					subset=new ModelSubset();
					subset->FaceCount=1;
					subset->FaceIndex=i;
					subset->MaterialIndex=index;
					original->Subset.push_back(subset);
					prev_index=index;
				} else {
					subset->FaceCount++;
				}
			}

			// �}�e���A��
			for (DWORD i=0; i<material_count; i++) {
				ModelMaterial* material=new ModelMaterial();
				Material.push_back(material);
				GetToken("Material");
				GetToken("{");
			
				// �F
//				material->Color=D3DXVECTOR4(
//					GetFloatToken(), GetFloatToken(), 
//					GetFloatToken(), GetFloatToken());
				material->Color.x = GetFloatToken();
				material->Color.y = GetFloatToken();
				material->Color.z = GetFloatToken();
				material->Color.w = GetFloatToken();
				for (int j=0; j<7; j++) GetToken();
				
				// �e�N�X�`��
				GetToken();
				if (strcmp(Token, "TextureFilename")==0) {
					GetToken("{");
					GetToken();
					material->Texture=GetTexture(pDevice, Token);
					GetToken("}");
					GetToken("}");
				}else{
//					GetToken("}");
					material->Texture=NULL;
				}
			}
		} else

		// �X�L���E�F�C�g
		if (strcmp(Token, "SkinWeights")==0) {
			GetToken("{");
			GetToken();
			MeshFrame* frame=FindFrame(Token);
			DWORD count=GetIntToken();
			DWORD* vertex=new DWORD[count];
			float* weight=new float[count];
			float matrix[16];
			for (DWORD i=0; i<count; i++) vertex[i]=GetIntToken();
			for (DWORD i=0; i<count; i++) weight[i]=GetFloatToken();
			for (DWORD i=0; i<16; i++) matrix[i]=GetFloatToken();
			GetToken("}");

			if (frame) {
				void* vb;

				original->VertexBuffer->Lock(0, 0, &vb, 0);
				ORIGINAL_VERTEX* v=(ORIGINAL_VERTEX*)vb;
				for (DWORD i=0; i<count; i++) {
					DWORD j=0;
					while (j<3 && v[vertex[i]].Weight[j]>0 && v[vertex[i]].Weight[j]<1) j++;
					if (j<3) {
						v[vertex[i]].Weight[j]=weight[i];
						v[vertex[i]].Index[j]=(unsigned char)frame->Index;
					}
				}
				original->VertexBuffer->Unlock();
				
				frame->OffsetMatrix=D3DXMATRIXA16(matrix);
			}
			delete[] vertex;
			delete[] weight;
		}
	}

	return S_OK;
}

// �t���[���̍X�V
void AnimMesh::AnimateFrame(D3DXMATRIXA16* world) {

	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s���������
	for (size_t i=0; i<Animation.size(); i++) {
		ModelAnimation* anim=Animation[i];
		if (anim->Weight==0) continue;

		// 0�ŏ�����
		for (size_t j=0; j<anim->Key.size(); j++) {
			ModelAnimationKey* key=anim->Key[j];
			ZeroMemory(&Frame[key->FrameIndex]->TransformMatrix, sizeof(D3DXMATRIXA16));
		}
	}

	// �A�j���[�V�����L�[���֌W����t���[���̕ό`�s����v�Z
	for (size_t i=0; i<Animation.size(); i++) {
		ModelAnimation* anim=Animation[i];
		if (anim->Weight==0) continue;
		
		for (size_t j=0; j<anim->Key.size(); j++) {
			ModelAnimationKey* key=anim->Key[j];
			MeshFrame* frame=Frame[key->FrameIndex];
			if (key->Time.empty()) continue;
			
			// �L�[�t���[���̕��
			float time=anim->Time;
			if (time<key->Time.front()) {
				frame->TransformMatrix+=*key->Matrix.front()*anim->Weight;
			} else
			if (time>=key->Time.back()) {
				frame->TransformMatrix+=*key->Matrix.back()*anim->Weight;
			} else {
				for (size_t k=1; k<key->Time.size(); k++) {
					if (time<key->Time[k] && key->Time[k-1]!=key->Time[k]) {
						float r=(key->Time[k]-time)/(key->Time[k]-key->Time[k-1]);
						frame->TransformMatrix+=
							(*key->Matrix[k-1]*r+*key->Matrix[k]*(1-r)) * anim->Weight;
						break;
					}
				}
			}
		}
	}

	// �t���[���̍����s����v�Z
	Frame[0]->Animate(world);

	// �A�j���[�V�����؂�ւ����
	if (SmootherWeight>0) {
		for (size_t i=0; i<Frame.size(); i++) {
			D3DXMATRIXA16& a=Frame[i]->CombinedMatrix;
			D3DXMATRIXA16& b=Frame[i]->SmootherMatrix;
			a=a*(1-SmootherWeight)+b*SmootherWeight;
			Frame[i]->SkinningMatrix=Frame[i]->OffsetMatrix*a;
		}
	}
}


// �t���[��
void MeshFrame::Animate(D3DXMATRIXA16* parent) {
	CombinedMatrix=TransformMatrix*(*parent);
	for (size_t i=0; i<Child.size(); i++) {
		Child[i]->Animate(&CombinedMatrix);
	}
	SkinningMatrix=OffsetMatrix*CombinedMatrix;
}


// ���_�ϊ�
void AnimMesh::AnimateVertex() {
	void* ovb;
	original->VertexBuffer->Lock(0, 0, &ovb, D3DLOCK_READONLY);			
	ORIGINAL_VERTEX* ov=(ORIGINAL_VERTEX*)ovb;

	void* avb;
	AnimatedVertexBuffer->Lock(0, 0, &avb, 0);			
	ANIMATED_VERTEX* av=(ANIMATED_VERTEX*)avb;

	for (DWORD i=0; i<original->VertexCount; i++) {
	
		// ���_
		D3DXVECTOR4 pos=D3DXVECTOR4(0, 0, 0, 0);
		for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) {
			D3DXVECTOR4 v;
			D3DXVec3Transform(&v, &ov[i].Pos, &Frame[ov[i].Index[j]]->SkinningMatrix);//�I���W�i�����_��ϊ�
			pos+=v*ov[i].Weight[j];
		}
		av[i].Pos=(D3DXVECTOR3)pos;//�A�j���[�V�����̒��_�ɏ�������

		// �@��
		D3DXVECTOR4 normal=D3DXVECTOR4(0, 0, 0, 0);
		for (DWORD j=0; j<MODEL_BLEND_COUNT; j++) {
			D3DXVECTOR3& n=ov[i].Normal;
			D3DXVECTOR4 v(n.x, n.y, n.z, 0);
			D3DXVec4Transform(&v, &v, &Frame[ov[i].Index[j]]->SkinningMatrix);//�I���W�i���@����ϊ�
			normal+=v*ov[i].Weight[j];
		}
		D3DXVec4Normalize(&normal, &normal);
		av[i].Normal=(D3DXVECTOR3)normal;//�A�j���[�V�����@���ɏ�������
	}

	original->VertexBuffer->Unlock();
	AnimatedVertexBuffer->Unlock();	
}


MeshFrame* AnimMesh::FindFrame(const char* name) {
	for (size_t i=0; i<Frame.size(); i++) {
		if (strcmp(Frame[i]->Name.c_str(), name)==0) return Frame[i];
	}
	return NULL;
}
ModelAnimation* AnimMesh::FindAnimation(const char* name) {
	for (size_t i=0; i< Animation.size(); i++) {
		if (strcmp(Animation[i]->Name.c_str(), name)==0) return Animation[i];
	}
	return NULL;
}

void AnimMesh::AddAnimation(ModelAnimation* anim) {
	Animation.push_back(new ModelAnimation(*anim));
}

void AnimMesh::ResetAnimationWeight() {
	for (size_t i=0; i<Animation.size(); i++) {
		Animation[i]->Weight=0;
	}
}

void AnimMesh::SaveSmoother() {
	for (size_t i=0; i<Frame.size(); i++) {
		Frame[i]->SmootherMatrix=Frame[i]->CombinedMatrix;
	}
}

AnimMesh* AnimMesh::Clone() {
	//// shallow�R�s�[
	AnimMesh* clone = new AnimMesh(*this);

	//// deep�R�s�[���K�v�ȃ����o�̃R�s�[
	// AnimatedVertexBuffer
	clone->AnimatedVertexBuffer = NULL;
	pDevice->CreateVertexBuffer(
				sizeof(ANIMATED_VERTEX)*original->VertexCount, 0, 
				ANIMATED_FVF, D3DPOOL_MANAGED,
				&clone->AnimatedVertexBuffer, NULL);

	ANIMATED_VERTEX* av_o;
	this->AnimatedVertexBuffer->Lock(0, 0, (void**)&av_o, 0);		

	ANIMATED_VERTEX* av_c;
	clone->AnimatedVertexBuffer->Lock(0, 0, (void**)&av_c, 0);

	for (DWORD i=0; i < original->VertexCount; i++) {
		av_c[i] = av_o[i];
	}
	this->AnimatedVertexBuffer->Unlock();
	clone->AnimatedVertexBuffer->Unlock();

	// Material
	for (size_t i = 0; i < Material.size(); ++i) {
		clone->Material[i] = new ModelMaterial(*this->Material[i]);
	}

	// Animation
	for (size_t i = 0; i < Animation.size(); ++i) {
		clone->Animation[i] = new ModelAnimation(*this->Animation[i]);
	}


	// MeshFrame
	clone->Frame.clear();
	MeshFrame* tmpCloneFrame = new MeshFrame();
	tmpCloneFrame->FrameClone(Frame.front(),&(clone->Frame));

	//// �Q�ƃJ�E���g�𑝉�
	original->RefCount++;

	return clone;
}


void AnimMesh::SetHitAttack(Hit* attack,int size){
	for(int i=0;i<size;i++){
		attack[i].FrameIndex=FindFrame(attack[i].FrameName)->Index;
		original->HitAttack.push_back(attack[i]);
	}
}
void AnimMesh::SetHitDefense(Hit* defense,int size){
	for(int i=0;i<size;i++){
		defense[i].FrameIndex=FindFrame(defense[i].FrameName)->Index;
		original->HitDefense.push_back(defense[i]);
	}
}
HRESULT AnimMesh::CalcBoundingSphere(){
	HRESULT hr;

	void*                    pVertices = NULL;  //�������o�b�t�@�̃|�C���^

	if(FAILED(hr = original->VertexBuffer->Lock(
		0,              //�S�̂����b�N
		0,              //�S�̂����b�N
		&pVertices,
		0)))
	{
		return hr;
	}

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		original->VertexCount,    //���_��
		D3DXGetFVFVertexSize(ORIGINAL_FVF),
		&original->center, &original->radius);  //�Ԃ���钆�S���W�Ɣ��a

	//���b�N����
	original->VertexBuffer->Unlock();

    return hr;

}
HRESULT AnimMesh::CalcBoundingBox(){
	HRESULT hr;

	void*                    pVertices = NULL;  //�������o�b�t�@�̃|�C���^

	if(FAILED(hr = original->VertexBuffer->Lock(
		0,              //�S�̂����b�N
		0,              //�S�̂����b�N
		&pVertices,
		0)))
	{
		return hr;
	}

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		original->VertexCount,    //���_��
		D3DXGetFVFVertexSize(ORIGINAL_FVF),
		&original->min, &original->max);  //�Ԃ���钆�S���W�Ɣ��a

	//���b�N����
	original->VertexBuffer->Unlock();

    return hr;

}
vector<Hit>* AnimMesh::getHitAttack(){
	return &(original->HitAttack);
}
vector<Hit>* AnimMesh::getHitDefense(){
	return &(original->HitDefense);
}
void AnimMesh::GetBoundingSphere(D3DXVECTOR3* center,float* radius){
	*center = original->center;
	*radius = original->radius;
}
void AnimMesh::GetBoundingBox(D3DXVECTOR3 *min, D3DXVECTOR3 *max){
	*min = original->min;
	*max = original->max;
}

HRESULT GetToken(){

	char* p=Pointer;
	char* q=Token;
	while (*p!='\0' && strchr(" \t\r\n,;\"", *p)) p++;
	if (*p=='{' || *p=='}') {
		*q++=*p++;
	} else {
		while (*p!='\0' && !strchr(" \t\r\n,;\"{}", *p)) *q++=*p++;
	}
	Pointer=p;
	*q='\0';

	return S_OK;
}
HRESULT GetToken(const char* token){

	GetToken();
	if (strcmp(Token, token)!=0) {
		char s[1024];
		sprintf_s(s, sizeof(s), "�z��g�[�N���F%s\n�A�ǂݍ��݃g�[�N���F%s", token, Token);
		MessageBox(NULL, s, "���f���̓ǂݍ��݃G���[", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}


float GetFloatToken(){
	GetToken();
	return (float)atof(Token);
}
int GetIntToken(){
	GetToken();
	return atoi(Token);
}
HRESULT SkipNode() {
	while (*Pointer!='\0') {
		GetToken();
		if (strchr(Token, '{')) break;
	}
	int count=1;
	while (*Pointer!='\0' && count>0) {
		GetToken();
		if (strchr(Token, '{')) count++; else
		if (strchr(Token, '}')) count--;
	}
	if (count>0) {
		MessageBox(NULL, "���ʂ̔�Ή�", "���f���̓ǂݍ��݃G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


