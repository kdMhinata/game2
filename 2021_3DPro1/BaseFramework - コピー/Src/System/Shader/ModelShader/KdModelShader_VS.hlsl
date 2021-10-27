#include "../inc_KdCommon.hlsli"
#include "inc_KdModelShader.hlsli"

VS_Output main(float4 pos : POSITION,
	float2 uv : TEXCOORD0,
	float3 normal:NORMAL,
	float4 color :COLOR,
	float3 tangent :TANGENT
)
{
	//構造体を戻り値にする
	VS_Output Out;
	//ワールド変換(Characterの位置へ変換)
	float4 wpos = mul(pos,g_mW);

	//ビュー変換(カメラと逆方向に移動&変換)
	wpos = mul(wpos, g_mView);

	//3D座標系->射影座標へ変換
	//射影変換(xy:-1~1, z:0~1の範囲に変換)
	wpos = mul(wpos, g_mProj);

	//構造体に値をコピー
	Out.Pos = wpos;

	//頂点色
	Out.Color = color;

	//UV座標はそのまま返す
	Out.UV = uv;

	//法線
	Out.Normal = normalize(mul(normal, (float3x3)g_mW));

	//接線
	Out.wT = normalize(mul(tangent,(float3x3)g_mW));

	//従法線
	Out.wB = normalize(mul(cross(normal,tangent),(float3x3)g_mW));

	return Out;
}