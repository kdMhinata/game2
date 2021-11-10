#include "inc_GenerateShadowMap.hlsli"

float4 main(VSOutput In) : SV_TARGET
{
	return float4(In.ProjPos.z / In.ProjPos.w, 0.0f, 0.0f, 1.0f);
}