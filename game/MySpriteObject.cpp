#include "MySpriteObject.h"
#include "Common.h"
#include "Error.h"

MySpriteObject::MySpriteObject(int width,int height,
						   CenterPositionFlag center,
						   LPDIRECT3DTEXTURE9 tex,
						   D3DXVECTOR3 position,
						   float Y,float R,float P)
					   :Object(position,Y,R,P,D3DXVECTOR3((float)width,(float)height,1.0f)),
					   width(width),height(height),
					   pTex(tex),myTex(false),color(D3DXCOLOR(255))
{
	// 4つの頂点からなる頂点バッファを作る
	if( FAILED( GetDevice()->CreateVertexBuffer( 
		4*sizeof(SPRITE_VERTEX),	// 頂点バッファ全体のサイズ
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使われ方
        SPRITE_VERTEX_FVF,			// 頂点データのFVFフォーマット
        D3DPOOL_MANAGED, 		// システムの頂点バッファ扱い方
		&pMyVB, 				// 頂点バッファを格納する変数
		NULL ) ) )				// 常にNULLに指定
	{
        return;
	}


    //頂点バッファーの中身を埋める
    SPRITE_VERTEX* v;
    pMyVB->Lock( 0, 0, (void**)&v, 0 );

	switch(center){
		case CPF_LEFT_TOP:
			v[0].p = D3DXVECTOR3(0		,-1.0f	,0.0f );	// 頂点①
			v[1].p = D3DXVECTOR3(0		,0.0f	,0.0f );	// 頂点②
			v[2].p = D3DXVECTOR3(1.0f	,-1.0f	,0.0f );	// 頂点③
			v[3].p = D3DXVECTOR3(1.0f	,0.0f	,0.0f );	// 頂点④
			break;
		default:
		case CPF_LEFT_BOTTOM:
			v[0].p = D3DXVECTOR3(0		,0		,0.0f );	// 頂点①
			v[1].p = D3DXVECTOR3(0		,1.0f	,0.0f );	// 頂点②
			v[2].p = D3DXVECTOR3(1.0f	,0		,0.0f );	// 頂点③
			v[3].p = D3DXVECTOR3(1.0f	,1.0f	,0.0f );	// 頂点④
			break;
		case CPF_CENTER:
			v[0].p = D3DXVECTOR3(-0.5f, -0.5f, 0.0f );	// 頂点①
			v[1].p = D3DXVECTOR3(-0.5f,  0.5f, 0.0f );	// 頂点②
			v[2].p = D3DXVECTOR3( 0.5f, -0.5f, 0.0f );	// 頂点③
			v[3].p = D3DXVECTOR3( 0.5f,  0.5f, 0.0f );	// 頂点④
			break;
		case CPF_RIGHT_TOP:
			v[0].p = D3DXVECTOR3(-1.0f	,-1.0f	,0.0f );	// 頂点①
			v[1].p = D3DXVECTOR3(-1.0f	,0.0f	,0.0f );	// 頂点②
			v[2].p = D3DXVECTOR3(0.0f	,-1.0f	,0.0f );	// 頂点③
			v[3].p = D3DXVECTOR3(0.0f	,0.0f	,0.0f );	// 頂点④
			break;
		case CPF_RIGHT_BOTTOM:
			v[0].p = D3DXVECTOR3(-1.0f	,0.0f	,0.0f );	// 頂点①
			v[1].p = D3DXVECTOR3(-1.0f	,1.0f	,0.0f );	// 頂点②
			v[2].p = D3DXVECTOR3(0.0f	,0.0f	,0.0f );	// 頂点③
			v[3].p = D3DXVECTOR3(0.0f	,1.0f	,0.0f );	// 頂点④
			break;
	}

	
	//頂点データ：頂点の座標値の設定
    v[0].tu = 0.0f;	// 頂点①
    v[0].tv = 1.0f;	// 頂点①
    v[1].tu = 0.0f;	// 頂点②
    v[1].tv = 0.0f;	// 頂点②
    v[2].tu = 1.0f;	// 頂点③
    v[2].tv = 1.0f;	// 頂点③
    v[3].tu = 1.0f;	// 頂点④
    v[3].tv = 0.0f;	// 頂点④

    
	//頂点データ：頂点の基本的な色の設定（白色に設定）
	//v[0].color = v[1].color = v[2].color = v[3].color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
  
    pMyVB->Unlock();
}
MySpriteObject::~MySpriteObject(){
	if(myTex){
		SAFE_RELEASE(pTex);
	}
}



void MySpriteObject::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);
	GetDevice()->SetTexture(0,pTex);

	D3DMATERIAL9 mat;
	ZeroMemory(&mat,sizeof(mat));
	mat.Emissive=mat.Diffuse=mat.Ambient=D3DXCOLOR(color);
	GetDevice()->SetMaterial(&mat);

	// 頂点バッファーをレンダリングパイプラインに設定
	GetDevice()->SetStreamSource( 0, pMyVB, 0, sizeof(SPRITE_VERTEX) );

	// 頂点データのＦＶＦフォーマットを設定
	GetDevice()->SetFVF( SPRITE_VERTEX_FVF );

	// 描画
	GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
HRESULT MySpriteObject::SetupTexture(char* filename){

	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFile(GetDevice(), filename,&pTex))){
			DisplayErrMsg( "テクスチャの読み込みに失敗しました");
			return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "すでにテクスチャが登録されている2Dオブジェクトにテクスチャを読み込もうとしました。");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}HRESULT MySpriteObject::SetupTexture(char* filename,int width,int height){

	if(!pTex && !myTex){
		if(FAILED(D3DXCreateTextureFromFileEx( GetDevice(), filename,width,height,
			0, 0, D3DFMT_UNKNOWN,D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,&pTex))){
				DisplayErrMsg( "テクスチャの読み込みに失敗しました");
				return(E_FAIL);
		}
	}else{
		DisplayErrMsg( "すでにテクスチャが登録されている2Dオブジェクトにテクスチャを読み込もうとしました。");
		return(E_FAIL);
	}
	myTex = true;
	return S_OK;
}