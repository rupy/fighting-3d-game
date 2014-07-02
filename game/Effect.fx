//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 	g_world;
float4x4 	g_world_inv;
float4x4 	g_view;
float4x4 	g_proj;
float4		g_color0;
float4		g_color1;
float4		g_light_dir;
texture		g_texture;

//------------------------------------------------
// テクスチャサンプラ
//------------------------------------------------
sampler TextureSampler = 
sampler_state
{
    Texture = <g_texture>;
	MipFilter = NONE;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

	AddressU =Clamp;
	AddressV =Clamp;
};	

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void BasicVS( 	float3 in_pos : POSITION,
				float3 in_normal : NORMAL,
				float2 in_tex : TEXCOORD0,
 				out float4 out_pos : POSITION,
 				out float4 out_color :COLOR0,
 				out float2 out_tex : TEXCOORD0)
{
	// 座標変換
	float4 world_pos;
	world_pos = mul(float4(in_pos, 1.0f), g_world);
	out_pos = mul(world_pos, g_view);
	out_pos = mul(out_pos, g_proj);

	// テクスチャ座標
	out_tex = in_tex;
	
	// 頂点ごとの色の計算
	float4 local_light;
	local_light =  normalize( mul(g_light_dir, g_world_inv));
	out_color = saturate(g_color0 + g_color1 * max(0, dot(local_light, in_normal)));
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void BasicPS( float4 in_color : COLOR0,
 			  float2 in_tex : TEXCOORD0,
			  out float4 out_color : COLOR0,
			  uniform bool b_tex)
{
	if(b_tex){
		out_color = in_color * tex2D(TextureSampler,in_tex);
	}
	else{
		out_color = in_color;
	}
}

//------------------------------------------------
// テクニック宣言
//------------------------------------------------
technique BasicTech
{
    pass P0
    {
        vertexShader = compile vs_3_0 BasicVS();
        pixelShader  = compile ps_3_0 BasicPS(true);
        
	}
    pass P1
    {
        vertexShader = compile vs_3_0 BasicVS();
        pixelShader  = compile ps_3_0 BasicPS(false);
        
	}

}