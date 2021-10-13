#include "../inc_KdCommon.hlsli"

#include "inc_GenerateShadowMap.hlsli"

cbuffer cbObject : register(b0)
{
	row_major float4x4 g_mWorld;
}

VSOutput main(float4 pos : POSITION)
{
	VSOutput Out;

	Out.Pos = pos;

	Out.Pos = mul(pos, g_mWorld);	//�Q�[����3D��Ԃ̍��Âɒu��
	Out.Pos = mul(Out.Pos, g_mView);//�J�������猩�����_�̍��W�ɒu��
	Out.Pos = mul(Out.Pos, g_mProj);//�ˉe���W�n(2d�����`�̉摜)�ϊ�

	//Out.UV = uv * g_UVTiling + g_UVOffset;

	//Out.Color = color;

	return Out;
}