//定数buffer受け取り準備
cbuffer cb: register(b0)
{
	//CPU側
	row_major float4x4 g_mW;
};

//マテリアル情報受け取り準備
struct Material
{
	float4 BaseColor;
};
cbuffer cbMaterial : register(b1)
{
	Material g_Material;
}

struct VS_Output
{
	float4 Pos :SV_POSITION;
	float2 UV:TEXCOORD0;	 //テクスチャのどこの色を使うか
	float3 Normal: TEXCOORD1; //法線ベクトル
};
