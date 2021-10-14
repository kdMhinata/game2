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
};
