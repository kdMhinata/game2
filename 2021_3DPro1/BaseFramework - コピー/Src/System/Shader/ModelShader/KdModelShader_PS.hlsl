#include "inc_KdModelShader.hlsli"

//�e�N�X�`���f�[�^
Texture2D g_baseTex : register(t0);

//�T���v��(�e�N�X�`��������𔲂��o���@�\)
SamplerState g_ss : register(s0);

float4 main(VS_Output In) : SV_TARGET
{
	return g_baseTex.Sample(g_ss,In.UV)* g_Material.BaseColor;
}