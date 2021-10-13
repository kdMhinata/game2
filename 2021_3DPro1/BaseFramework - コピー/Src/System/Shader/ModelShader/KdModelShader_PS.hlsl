#include "inc_KdModelShader.hlsli"

//テクスチャデータ
Texture2D g_baseTex : register(t0);

//サンプラ(テクスチャから情報を抜き出す機能)
SamplerState g_ss : register(s0);

float4 main(VS_Output In) : SV_TARGET
{
	return g_baseTex.Sample(g_ss,In.UV)* g_Material.BaseColor;
}