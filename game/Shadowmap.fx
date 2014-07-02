//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 	g_vlp;
float4x4 	g_world;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex shader
//--------------------------------------------------------------------------------------
void BasicShadowBufVS( 	float4 in_pos : POSITION,
						out float4 out_pos : POSITION,
						out float4 out_depth : TEXCOORD0)
{	
	// 座標変換
	out_pos = mul(mul(in_pos, g_world),g_vlp);

	// 位置情報の出力
	out_depth  = out_pos;
}


//--------------------------------------------------------------------------------------
// Pixel shader
//--------------------------------------------------------------------------------------
float4 BasicShadowBufPS(	float4 in_depth : TEXCOORD0): COLOR
{
	// ピクセルの深度情報を計算して出力
	float4 depthcolor= in_depth.z / in_depth.w;
	return depthcolor;
}

//--------------------------------------------------------------------------------------
// Pixel shader
//--------------------------------------------------------------------------------------
technique BasicTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 BasicShadowBufVS();
        PixelShader  = compile ps_2_0 BasicShadowBufPS();
	}
}
