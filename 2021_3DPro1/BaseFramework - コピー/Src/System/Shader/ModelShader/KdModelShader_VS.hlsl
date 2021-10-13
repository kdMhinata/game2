#include "../inc_KdCommon.hlsli"
#include "inc_KdModelShader.hlsli"

VS_Output main(float4 pos : POSITION,
	float2 uv : TEXCOORD0)
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

	//UV座標はそのまま返す
	Out.UV = uv;

	return Out;
}