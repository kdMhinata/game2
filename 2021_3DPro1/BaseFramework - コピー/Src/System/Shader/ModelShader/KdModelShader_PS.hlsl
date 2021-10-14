#include "inc_KdModelShader.hlsli"
#include "../inc_KdCommon.hlsli"

//�e�N�X�`���f�[�^
Texture2D g_baseTex : register(t0);

//�T���v��(�e�N�X�`��������𔲂��o���@�\)
SamplerState g_ss : register(s0);

float4 main(VS_Output In) : SV_TARGET
{
	//�@�����K��
	//�Q�_�Ԃ̒��_�̖@�����g���ĐV�����@�������
	float wN = normalize(In.Normal);

	//�ގ��F
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor;

	//���C�e�B���O
	//�ŏI�I�ȐF
	float3 color = 0;

	//Diffuse(�g�U��)
	{
		//���̕����Ɩ@���̕����Ƃ̊p�x�����̋����ɂȂ�
		float lightDiffuse = dot(-g_DL_Dir, wN);
		lightDiffuse = saturate(lightDiffuse);

		//���K��Lambert
		lightDiffuse /= 3.1415926535;

		//���̐F*�ގ��̊g�U�F*������
		color += (g_DL_Color * lightDiffuse) * baseColor.rgb * baseColor.a;
	}

	color += g_AmbientLight * baseColor.rgb * baseColor.a;

	//�o��
	return float4(color, baseColor.a);
}