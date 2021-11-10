#include "inc_KdModelShader.hlsli"
#include "../inc_KdCommon.hlsli"

//�e�N�X�`���f�[�^
Texture2D g_baseTex : register(t0);
Texture2D g_mrTex : register(t1);
Texture2D g_normalTex: register(t2);

Texture2D g_dirLightShadowMap : register(t10);

//�T���v��(�e�N�X�`��������𔲂��o���@�\)
SamplerState g_ss : register(s0);

float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
	float3 H = normalize(-lightDir + vCam);
	float NdotH = saturate(dot(normal,H));
	float spec = pow(NdotH, specPower);

	//���K��BlinnPhong
	return spec * ((specPower + 2) / (2 * 3.1415926525));
}

float4 main(VS_Output In) : SV_TARGET
{
	//�J�����ւ̕���
	float3 vCam = g_CamPos - In.Pos;
	float camDist = length(vCam);
	vCam = normalize(vCam);

	//3��̖@������@���s����쐬
	row_major float3x3 mTBN = {
		normalize(In.wT),
		normalize(In.wB),
		normalize(In.Normal),
	};

	//�@���}�b�v����@���x�N�g�����擾
	float3 wN = g_normalTex.Sample(g_ss, In.UV).rgb;

	//0~1����-1=1�֕ϊ�
	wN = wN * 2.0 - 1.0;

	//�@���x�N�g�������̃s�N�Z����Ԃ֕ϊ�
	wN = normalize(mul(wN, mTBN));

	//�ގ��F
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor*In.Color;
	float4 mr = g_mrTex.Sample(g_ss, In.UV);

	//������
	float metallic = g_Material.Metallic;

	//�e��
	float roughness = g_Material.Roughness;

	//���t�l�X���t�]�����Ȃ߂炩�����Z�o����@1=�Ȃ߂炩~0=���炴��
	float smoothness = 1.0 - roughness;

	//1~2048�̒l
	float specPower = pow(2, 11 * smoothness);

	//���C�e�B���O
	//�ŏI�I�ȐF
	float3 color = 0;

	//�ގ��̊g�U�F(������قǍގ��̐F�ɂȂ�A�����قǊg�U�F�͖����Ȃ�)
	const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);

	//�ގ��̔��ːF(������قǌ��̐F�����̂܂ܔ��˂��A�������ގ��̐F�����)
	const float3 bassSpecular = lerp(0.04,baseColor.rgb,metallic);

	float shadow = 1.0;

	//���C�g�̃r���[�s��Ǝˉe�s��ŕϊ�
	float4 dlPos = mul(float4(In.wPos, 1), g_DL_mViewProj);

	//�[�x�����߂�
	dlPos = dlPos.xyz / dlPos.w;

	if (abs(dlPos.x) <= 1.0 && abs(dlPos.y) <= 1.0 && abs(dlPos.z) <= 1.0)
	{
		float2 uv = dlPos.xy * float2(1, -1) * 0.5f + 0.5f;

		shadow = m_dirLightShadowMap.Sample(g_ss, uv).r < dlPos.z ? 0 : 1.0;
	}


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

	color += g_AmbientLight * baseColor.rgb * baseColor.a * shadow;

	//���ˌ�
	{
		float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

		color += (g_DL_Color * spec) * baseColor.a * 0.5;
	}

	//�o��
	return float4(color, baseColor.a);
}