#ifndef __VERTEXOBJECT_H__
#define __VERTEXOBJECT_H__

#include "Object.h"
#include "Effect.h"

class VertexObject :public Object
{
public:
	LPDIRECT3DVERTEXBUFFER9		pMyVB;// 頂点バッファー
	LPDIRECT3DTEXTURE9 pTex;
	bool myTex;
	vector<Effect*>&	rEff;

	float alpha;
public:

	VertexObject(vector<Effect*>& eff,
		LPDIRECT3DTEXTURE9 tex=NULL,
		D3DXVECTOR3 position=D3DXVECTOR3(0,0,0),
		float Y=0.0f,float R=0.0f,float P=0.0f);

	~VertexObject();
	//自分を描画
	void Appear();
	HRESULT SetupTexture(char* filename);
	HRESULT SetupTexture(char* filename,int width,int height);
//private:
		// 頂点データの構造体
	struct MY_VERTEX{
		D3DXVECTOR3 p;		// 位置
		float tu,tv;//テクスチャ
	};
	// 頂点データのＦＶＦフォーマットの設定
	#define MY_VERTEX_FVF  (D3DFVF_XYZ|D3DFVF_TEX1 )
};
#endif	/*__VERTEXOBJECT_H__*/