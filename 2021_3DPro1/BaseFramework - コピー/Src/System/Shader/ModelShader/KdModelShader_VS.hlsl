#include "../inc_KdCommon.hlsli"
#include "inc_KdModelShader.hlsli"

VS_Output main(float4 pos : POSITION,
	float2 uv : TEXCOORD0,
	float3 normal:NORMAL,
	float4 color :COLOR,
	float3 tangent :TANGENT
)
{
	//�\���̂�߂�l�ɂ���
	VS_Output Out;
	//���[���h�ϊ�(Character�̈ʒu�֕ϊ�)
	float4 wpos = mul(pos,g_mW);

	//�r���[�ϊ�(�J�����Ƌt�����Ɉړ�&�ϊ�)
	wpos = mul(wpos, g_mView);

	//3D���W�n->�ˉe���W�֕ϊ�
	//�ˉe�ϊ�(xy:-1~1, z:0~1�͈̔͂ɕϊ�)
	wpos = mul(wpos, g_mProj);

	//�\���̂ɒl���R�s�[
	Out.Pos = wpos;

	//���_�F
	Out.Color = color;

	//UV���W�͂��̂܂ܕԂ�
	Out.UV = uv;

	//�@��
	Out.Normal = normalize(mul(normal, (float3x3)g_mW));

	//�ڐ�
	Out.wT = normalize(mul(tangent,(float3x3)g_mW));

	//�]�@��
	Out.wB = normalize(mul(cross(normal,tangent),(float3x3)g_mW));

	return Out;
}