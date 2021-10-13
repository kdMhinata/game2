#include "../inc_KdCommon.hlsli"

#include "inc_GenerateShadowMap.hlsli"

cbuffer cbObject : register(b0)
{
	row_major float4x4 g_mWorld;
}

VSOutput main(float4 pos : POSITION)
{
	VSOutput Out;

	Out.Pos = pos;

	Out.Pos = mul(pos, g_mWorld);	//ゲームの3D空間の座凝に置換
	Out.Pos = mul(Out.Pos, g_mView);//カメラから見た頂点の座標に置換
	Out.Pos = mul(Out.Pos, g_mProj);//射影座標系(2d正方形の画像)変換

	//Out.UV = uv * g_UVTiling + g_UVOffset;

	//Out.Color = color;

	return Out;
}