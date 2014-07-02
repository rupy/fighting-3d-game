#include "Font.h"
#include "Common.h"//for search window handle
#include <stdio.h>//for debuck
#include <tchar.h>

Font::Font()
{

}
Font::~Font(){}

HRESULT Font::MakeFont(LPDIRECT3DDEVICE9 device,const TCHAR* ch,int fontsize){

	
	const TCHAR *c = ch;

	if(*c == '\n'){//���s�����̏ꍇpTex��NULL������
		pTex = NULL;
		return S_OK;
	}
	HWND hWnd=FindWindow(WINDOW_TITLE,NULL);

	//�_���t�H���g�̍쐬
	LOGFONT lf;
	lf.lfHeight=fontsize;							// �����Z���܂��͕����̍���
	lf.lfWidth=0;									// ���ϕ�����
	lf.lfEscapement=0;								// ��������̕�����X���Ƃ̊p�x
	lf.lfOrientation=0;								// �x�[�X���C����X���Ƃ̊p�x
	lf.lfWeight=1000;									// �t�H���g�̑���
	lf.lfItalic=0;									// �C�^���b�N�̎w��
	lf.lfUnderline=0;								// �����t���w��
	lf.lfStrikeOut=0;								// �ł��������t���w��
	lf.lfCharSet=SHIFTJIS_CHARSET;					// �L�����N�^�Z�b�g
	lf.lfOutPrecision=OUT_TT_ONLY_PRECIS;			// �o�͐��x
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;			// �N���b�s���O�̐��x
	lf.lfQuality=PROOF_QUALITY;						// �o�͕i��
	lf.lfPitchAndFamily=FIXED_PITCH | FF_MODERN;	// �s�b�`�ƃt�@�~��
	strcpy(lf.lfFaceName,_T("�l�r ����"));				// �t�H���g��

	HFONT hFont;
	if(!(hFont = CreateFontIndirect(&lf))){
		return E_FAIL;
	}

	//�f�o�C�X�R���e�L�X�g�̎擾
	HDC hdc = GetDC(hWnd);	
	//�_���t�H���g�̗L����
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	UINT code = 0;

	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];

	//�t�H���g�̏ڍׂ��擾
	TEXTMETRIC TM;
	GetTextMetrics( hdc, &TM );

	// �t�H���g�r�b�g�}�b�v�擾
	GLYPHMETRICS GM;
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE *ptr = new BYTE[size];
//	memset(ptr,0,sizeof(BYTE)*size);
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	// ���_���
	float a = 1.0f; // �e�N�X�`���̏k��
	fTexW = GM.gmCellIncX * a; // �e�N�X�`���̉���
	fTexH = TM.tmHeight * a; // �e�N�X�`���̍���

	// �e�N�X�`���쐬
	if(FAILED(device->CreateTexture( GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL)))
	{
		delete[] ptr;
		return E_FAIL;
	}

	// �e�N�X�`���Ƀt�H���g�r�b�g�}�b�v��������
	D3DLOCKED_RECT LockedRect;
	if(FAILED(pTex->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))//{
		if(FAILED(pTex->LockRect(0, &LockedRect, NULL, 0)))
		{
			delete[] ptr;
			return E_FAIL;
		}
//	}

	// �t�H���g���̏�������
	// iOfs_x, iOfs_y : �����o���ʒu(����)
	// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
	int iOfs_x = GM.gmptGlyphOrigin.x;
	int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	int iBmp_w = GM.gmBlackBoxX + (4-(GM.gmBlackBoxX%4))%4;
	int iBmp_h = GM.gmBlackBoxY;
	int Level = 17;
	int x, y;
	DWORD Alpha, Color;
	FillMemory(LockedRect.pBits , LockedRect.Pitch * TM.tmHeight, 0);

	for(y=iOfs_y; y<iOfs_y+iBmp_h; y++){
		for(x=iOfs_x; x<iOfs_x+iBmp_w; x++){
			Alpha = (255 * ptr[x-iOfs_x + iBmp_w*(y-iOfs_y)]) / (Level-1);
			if(Alpha>0xff){Alpha=0x0;}
			Color = 0x00ffffff | (Alpha<<24);
			memcpy((BYTE*)LockedRect.pBits + LockedRect.Pitch*y + 4*x, &Color, sizeof(DWORD));
//	MyOutputDebugString( _T("<GAMEMODE:%lx,%lx>\n"),Color,Alpha);
		}
	}
	pTex->UnlockRect(0);
	delete[] ptr;

	return S_OK;
}