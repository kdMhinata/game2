#include "../inc_KdCommon.hlsli"

#include "inc_GenerateShadowMap.hlsli"

cbuffer cbObject : register(b0)
{
	row_major float4x4 g_mWorld;
}
cbuffer cbObject : register(b1)
{
	row_major float4x4 g_mLightView;
	row_major float4x4 g_mLightProj;
}

VSOutput main(float4 pos : POSITION)
{
	VSOutput Out;

	Out.Pos = pos;
	
	// ���W�ϊ�
	Out.Pos = mul(pos, g_mWorld);		// �Q�[���̂RD��Ԃ̍��W�ɒu��
	Out.Pos = mul(Out.Pos, g_mLightView);	// ���s�����猩�����_�̍��W�ɒu��
	Out.Pos = mul(Out.Pos, g_mLightProj);	// ���s���̐��ˉe���W�n(2D�����`�̉摜)�ϊ�

	// UV���W
	//Out.UV = uv * g_UVTiling + g_UVOffset;

	// �o��
	return Out;
}