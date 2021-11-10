#include "inc_KdModelShader.hlsli"
#include "../inc_KdCommon.hlsli"

//テクスチャデータ
Texture2D g_baseTex : register(t0);
Texture2D g_mrTex : register(t1);
Texture2D g_normalTex: register(t2);

Texture2D g_dirLightShadowMap : register(t10);

//サンプラ(テクスチャから情報を抜き出す機能)
SamplerState g_ss : register(s0);

float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
	float3 H = normalize(-lightDir + vCam);
	float NdotH = saturate(dot(normal,H));
	float spec = pow(NdotH, specPower);

	//正規化BlinnPhong
	return spec * ((specPower + 2) / (2 * 3.1415926525));
}

float4 main(VS_Output In) : SV_TARGET
{
	//カメラへの方向
	float3 vCam = g_CamPos - In.Pos;
	float camDist = length(vCam);
	vCam = normalize(vCam);

	//3種の法線から法線行列を作成
	row_major float3x3 mTBN = {
		normalize(In.wT),
		normalize(In.wB),
		normalize(In.Normal),
	};

	//法線マップから法線ベクトルを取得
	float3 wN = g_normalTex.Sample(g_ss, In.UV).rgb;

	//0~1から-1=1へ変換
	wN = wN * 2.0 - 1.0;

	//法線ベクトルをこのピクセル空間へ変換
	wN = normalize(mul(wN, mTBN));

	//材質色
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor*In.Color;
	float4 mr = g_mrTex.Sample(g_ss, In.UV);

	//金属性
	float metallic = g_Material.Metallic;

	//粗さ
	float roughness = g_Material.Roughness;

	//ラフネスを逆転させなめらかさを算出する　1=なめらか~0=ざらざら
	float smoothness = 1.0 - roughness;

	//1~2048の値
	float specPower = pow(2, 11 * smoothness);

	//ライティング
	//最終的な色
	float3 color = 0;

	//材質の拡散色(非金属ほど材質の色になり、金属ほど拡散色は無くなる)
	const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);

	//材質の反射色(非金属ほど光の色をそのまま反射し、金属程材質の色が乗る)
	const float3 bassSpecular = lerp(0.04,baseColor.rgb,metallic);

	float shadow = 1.0;

	//ライトのビュー行列と射影行列で変換
	float4 dlPos = mul(float4(In.wPos, 1), g_DL_mViewProj);

	//深度を求める
	dlPos = dlPos.xyz / dlPos.w;

	if (abs(dlPos.x) <= 1.0 && abs(dlPos.y) <= 1.0 && abs(dlPos.z) <= 1.0)
	{
		float2 uv = dlPos.xy * float2(1, -1) * 0.5f + 0.5f;

		shadow = m_dirLightShadowMap.Sample(g_ss, uv).r < dlPos.z ? 0 : 1.0;
	}


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

	color += g_AmbientLight * baseColor.rgb * baseColor.a * shadow;

	//反射光
	{
		float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

		color += (g_DL_Color * spec) * baseColor.a * 0.5;
	}

	//出力
	return float4(color, baseColor.a);
}