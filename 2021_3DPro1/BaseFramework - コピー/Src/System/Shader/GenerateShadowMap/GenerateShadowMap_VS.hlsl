#include "inc_GenerateShadowMap.hisli"

VSOutput main(float4 pos : POSITION)
{
	VSOUtput Out;
/*
Out.Pos = mul(pos,g_mWorld);
Out.Pos = mul(Out.Pos,g_mView);
Out.Pos = mul(Out.Pos, g_mProj);
*/
return Out;
}