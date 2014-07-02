#include "MeasureLine.h"

MeasureLine::MeasureLine(int domain_start,int domain_end,int range_start,int range_end)
:domain_start(domain_start),domain_end(domain_end),range_start(range_start),range_end(range_end)
{}

void MeasureLine::Appear(){

	D3DXMATRIXA16 matWorld = MatrixSetting();
	GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);

	GetDevice()->SetVertexShader(NULL);
	GetDevice()->SetFVF(D3DFVF_XYZ);
	GetDevice()->SetTexture(0,NULL);

	D3DMATERIAL9 material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse.a = 255;
	material.Diffuse.r = 255;
	material.Diffuse.g = 255;
	material.Diffuse.b = 255;
	material.Ambient = material.Diffuse;
	material.Emissive = material.Diffuse;

	GetDevice()->SetMaterial(&material);

	D3DXVECTOR3 vecPoint[3];

	//縦線（Z軸に平行な線）
	for(int i=domain_start;i<=domain_end;i++){
		vecPoint[0] = D3DXVECTOR3((float)i,0.0f,(float)range_start);
		vecPoint[1] = D3DXVECTOR3((float)i,0.0f,(float)range_end);

		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));

	}
	//横軸（X軸に平行な線）
	for(int i=range_start;i<=range_end;i++){
		vecPoint[0] = D3DXVECTOR3((float)domain_start,0.0f,(float)i);
		vecPoint[1] = D3DXVECTOR3((float)domain_end,0.0f,(float)i);

		GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,1,vecPoint,sizeof(D3DXVECTOR3));
	}

	
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