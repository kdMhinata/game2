#include "../inc_KdCommon.hlsli"

float4 main(float4 pos : POSITION) : SV_POSITION
{

	float4 _pos = pos;

	//3D���W�n->�ˉe���W�֕ϊ�
	_pos = mul(pos, g_mProj);
		return _pos;
}