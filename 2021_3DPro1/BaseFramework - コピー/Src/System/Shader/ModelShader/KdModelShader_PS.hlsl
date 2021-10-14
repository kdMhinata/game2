#include "inc_KdModelShader.hlsli"
#include "../inc_KdCommon.hlsli"

//テクスチャデータ
Texture2D g_baseTex : register(t0);

//サンプラ(テクスチャから情報を抜き出す機能)
SamplerState g_ss : register(s0);

float4 main(VS_Output In) : SV_TARGET
{
	//法線正規化
	//２点間の頂点の法線を使って新しい法線を作る
	float wN = normalize(In.Normal);

	//材質色
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor;

	//ライティング
	//最終的な色
	float3 color = 0;

	//Diffuse(拡散光)
	{
		//光の方向と法線の方向との角度が光の強さになる
		float lightDiffuse = dot(-g_DL_Dir, wN);
		lightDiffuse = saturate(lightDiffuse);

		//正規化Lambert
		lightDiffuse /= 3.1415926535;

		//光の色*材質の拡散色*透明率
		color += (g_DL_Color * lightDiffuse) * baseColor.rgb * baseColor.a;
	}

	color += g_AmbientLight * baseColor.rgb * baseColor.a;

	//出力
	return float4(color, baseColor.a);
}