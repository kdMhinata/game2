#include "../inc_KdCommon.hlsli"

float4 main(float4 pos : POSITION) : SV_POSITION
{

	float4 _pos = pos;

	//3DÀ•WŒn->Ë‰eÀ•W‚Ö•ÏŠ·
	_pos = mul(pos, g_mProj);
		return _pos;
}