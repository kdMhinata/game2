//’è”bufferó‚¯æ‚è€”õ
cbuffer cb: register(b0)
{
	//CPU‘¤
	row_major float4x4 g_mW;
};

//ƒ}ƒeƒŠƒAƒ‹î•ñó‚¯æ‚è€”õ
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
	float4 Pos :SV_PoSITION;
	float2 UV:TEXCOORD0;
};
