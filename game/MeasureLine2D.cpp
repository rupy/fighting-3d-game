#include "MeasureLine2D.h"
#include "Common.h"

MeasureLine2D::MeasureLine2D(int increment,int emphasis)
:increment(increment),emphasis(emphasis)
{}

void MeasureLine2D::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE);

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_XYZ);
	GetDevice()->SetTexture(0,NULL);

	D3DMATERIAL9 material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse.a = 1.0f;
	material.Diffuse.r = 0.5f;
	material.Diffuse.g = 0.5f;
	material.Diffuse.b = 0.5f;
	material.Ambient = material.Diffuse;
	material.Emissive = material.Diffuse;

	GetDevice()->SetMaterial(&material);

	D3DMATERIAL9 material_em;
	ZeroMemory(&material_em,sizeof(material_em));
	material_em.Diffuse.a = 1.0f;
	material_em.Diffuse.r = 1.0f;
	material_em.Diffuse.g = 1.0f;
	material_em.Diffuse.b = 1.0f;
	material_em.Ambient = material_em.Diffuse;
	material_em.Emissive = material_em.Diffuse;


	D3DXVECTOR3 vecPoint[3];

	//縦線（Z軸に平行な線）
	for(int i=0,e = 0;i<=WINDOW_WIDTH;i+=increment,e++){
		vecPoint[0] = D3DXVECTOR3((float)i,0,0.0f);
		vecPoint[1] = D3DXVECTOR3((float)i,(float)WINDOW_HEIGHT,0.0f);
		if(e >= emphasis){
			GetDevice()->SetMaterial(&material_em);
			GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
			GetDevice()->SetMaterial(&material);
			e=0;
			continue;
		}

		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));

	}
	//横軸（X軸に平行な線）
	for(int i=0,e = 0;i<=WINDOW_HEIGHT;i+=increment,e++){
		vecPoint[0] = D3DXVECTOR3(0,(float)i,0.0f);
		vecPoint[1] = D3DXVECTOR3((float)WINDOW_WIDTH,(float)i,0.0f);

		if(e >= emphasis){
			GetDevice()->SetMaterial(&material_em);
			GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
			GetDevice()->SetMaterial(&material);
			e=0;
			continue;
		}
		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
	}

	
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);
	/*ZeroMemory(&material,sizeof(material));
	material.Diffuse.a = 255;
	material.Diffuse.r = 0;
	material.Diffuse.g = 0;
	material.Diffuse.b = 255;
	material.Ambient = material.Diffuse;
	material.Emissive = material.Diffuse;

	GetDevice()->SetMaterial(&material);
	//放物線
	for(int i=range_start;i<range_end;i++){
		vecPoint[0] = D3DXVECTOR3((float)i,0.0f,(float)i*i*0.2f-range_end);
		vecPoint[1] = D3DXVECTOR3((float)i+1,0.0f,(float)(i+1)*(i+1)*0.2f-range_end);

		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
	}*/

}