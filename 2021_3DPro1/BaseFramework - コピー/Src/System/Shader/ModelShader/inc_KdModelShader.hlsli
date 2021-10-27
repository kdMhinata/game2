//�萔buffer�󂯎�菀��
cbuffer cb: register(b0)
{
	//CPU��
	row_major float4x4 g_mW;
};

//�}�e���A�����󂯎�菀��
struct Material
{
	float4 BaseColor;
	float Metallic;
	float Roughness;
};
cbuffer cbMaterial : register(b1)
{
	Material g_Material;
}

struct VS_Output
{
	float4 Pos :SV_POSITION;
	float2 UV:TEXCOORD0;	 //�e�N�X�`���̂ǂ��̐F���g����
	float3 Normal: TEXCOORD1; //�@���x�N�g��
	float4 Color :TEXCOORD2; //�F
	float3 wPos :TEXCOORD3; //���[���h3D���W
	float3 wT :TEXCOORD4; //���[���h�ڐ��iTangent)
	float3 wB :TEXCOORD5; //���[���h�]�@��(Binormal)
};
