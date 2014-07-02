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

	if(*c == '\n'){//改行文字の場合pTexにNULLを入れる
		pTex = NULL;
		return S_OK;
	}
	HWND hWnd=FindWindow(WINDOW_TITLE,NULL);

	//論理フォントの作成
	LOGFONT lf;
	lf.lfHeight=fontsize;							// 文字セルまたは文字の高さ
	lf.lfWidth=0;									// 平均文字幅
	lf.lfEscapement=0;								// 文字送りの方向とX軸との角度
	lf.lfOrientation=0;								// ベースラインとX軸との角度
	lf.lfWeight=1000;									// フォントの太さ
	lf.lfItalic=0;									// イタリック体指定
	lf.lfUnderline=0;								// 下線付き指定
	lf.lfStrikeOut=0;								// 打ち消し線付き指定
	lf.lfCharSet=SHIFTJIS_CHARSET;					// キャラクタセット
	lf.lfOutPrecision=OUT_TT_ONLY_PRECIS;			// 出力精度
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;			// クリッピングの精度
	lf.lfQuality=PROOF_QUALITY;						// 出力品質
	lf.lfPitchAndFamily=FIXED_PITCH | FF_MODERN;	// ピッチとファミリ
	strcpy(lf.lfFaceName,_T("ＭＳ 明朝"));				// フォント名

	HFONT hFont;
	if(!(hFont = CreateFontIndirect(&lf))){
		return E_FAIL;
	}

	//デバイスコンテキストの取得
	HDC hdc = GetDC(hWnd);	
	//論理フォントの有効化
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	UINT code = 0;

	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];

	//フォントの詳細を取得
	TEXTMETRIC TM;
	GetTextMetrics( hdc, &TM );

	// フォントビットマップ取得
	GLYPHMETRICS GM;
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE *ptr = new BYTE[size];
//	memset(ptr,0,sizeof(BYTE)*size);
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	// 頂点情報
	float a = 1.0f; // テクスチャの縮尺
	fTexW = GM.gmCellIncX * a; // テクスチャの横幅
	fTexH = TM.tmHeight * a; // テクスチャの高さ

	// テクスチャ作成
	if(FAILED(device->CreateTexture( GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL)))
	{
		delete[] ptr;
		return E_FAIL;
	}

	// テクスチャにフォントビットマップ書き込み
	D3DLOCKED_RECT LockedRect;
	if(FAILED(pTex->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))//{
		if(FAILED(pTex->LockRect(0, &LockedRect, NULL, 0)))
		{
			delete[] ptr;
			return E_FAIL;
		}
//	}

	// フォント情報の書き込み
	// iOfs_x, iOfs_y : 書き出し位置(左上)
	// iBmp_w, iBmp_h : フォントビットマップの幅高
	// Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)
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