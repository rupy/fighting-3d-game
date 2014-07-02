//=============================================================
// Renderer.cpp
// �����_���[�N���X�̎���
//=============================================================

#include "Renderer.h"
#include "Common.h"
//=============================================================
// ����
//=============================================================
//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
Renderer::Renderer()
 : pD3D(NULL), pDevice(NULL),pSprite(NULL)
{
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
Renderer::~Renderer()
{
	Finalize();
}

//-------------------------------------------------------------
// �����_���[�̏�����
// ����
//		hWnd         : �E�B���h�E�n���h��
//		isFullScreen : �t���X�N���[���̏ꍇTRUE
//		clientWidth  : �N���C�A���g�̈�̕�
//		clientHeight : �N���C�A���g�̈�̍���
// �߂�l
//		����������S_OK 
//-------------------------------------------------------------
HRESULT Renderer::Initialize(HWND hWnd, bool isFullScreen, int clientWidth, int clientHeight)
{

    D3DDISPLAYMODE d3ddm;
    
	// Direct3D9�I�u�W�F�N�g�̍쐬
	if((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
        return E_FAIL;	// �擾���s
    }
	
	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	//�f�o�C�X�̔\�͂𒲂ׂ�
	//D3DCAPS9 caps;
	//if(FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps))){
	//	return E_FAIL;
	//}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	D3DPP.BackBufferCount			= 1;
	if(isFullScreen) { // �t���X�N���[���̏ꍇ
		D3DPP.Windowed				= FALSE;			// �t���X�N���[���\���̎w��
		D3DPP.BackBufferWidth		= clientWidth;		// �t���X�N���[�����̉���
		D3DPP.BackBufferHeight		= clientHeight;		// �t���X�N���[�����̏c��
	}
	else {
		D3DPP.Windowed				= TRUE;				// �E�C���h�E���\���̎w��
	}
	D3DPP.BackBufferFormat		= d3ddm.Format;			// �J���[���[�h�̎w��
	D3DPP.SwapEffect				= D3DSWAPEFFECT_DISCARD; //�ł������I�ȃX���b�v�`�F�[���̕\����I��
	D3DPP.EnableAutoDepthStencil	= TRUE;
	D3DPP.AutoDepthStencilFormat	= D3DFMT_D16;
	D3DPP.hDeviceWindow				= hWnd;
	
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&D3DPP, &pDevice))) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&D3DPP, &pDevice))) {
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&D3DPP, &pDevice))) {
				// ���������s
				return E_FAIL;
			}
		}
	}

	//�X�v���C�g�̍쐬
	if(FAILED(D3DXCreateSprite(pDevice,&pSprite))){
		return(E_FAIL);
	}

	return S_OK;
}

//---------------------------------------------------------
// �f�o�C�X�̎擾
// �߂�l
//		����������IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
//		���s������0
//---------------------------------------------------------
LPDIRECT3DDEVICE9 Renderer::GetDevice()
{
	return pDevice;
}

D3DPRESENT_PARAMETERS* Renderer::GetPParam()
{
	return (&D3DPP);
}

LPD3DXSPRITE Renderer::GetSprite()
{
	return (pSprite);
}


//-------------------------------------------------------------
// �I������
//-------------------------------------------------------------
void Renderer::Finalize()
{
	// �f�o�C�X�I�u�W�F�N�g�̉��
	SAFE_RELEASE(pDevice);

	// DirectXGraphics�̉��
	SAFE_RELEASE(pD3D);

	//�X�v���C�g�̉��
	SAFE_RELEASE(pSprite);

}