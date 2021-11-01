#include "../inc_KdCommon.hlsli"

#include "inc_GenerateShadowMap.hlsli"

cbuffer cbObject : register(b0)
{
	row_major float4x4 g_mWorld;
}
cbuffer cbObject : register(b1)
{
	row_major float4x4 g_mLightView;
	row_major float4x4 g_mLightProj;
}

VSOutput main(float4 pos : POSITION)
{
	VSOutput Out;

	Out.Pos = pos;
	
	// 座標変換
	Out.Pos = mul(pos, g_mWorld);		// ゲームの３D空間の座標に置換
	Out.Pos = mul(Out.Pos, g_mLightView);	// 平行光から見た頂点の座標に置換
	Out.Pos = mul(Out.Pos, g_mLightProj);	// 平行光の正射影座標系(2D正方形の画像)変換

	// UV座標
	//Out.UV = uv * g_UVTiling + g_UVOffset;

	// 出力
	return Out;
}