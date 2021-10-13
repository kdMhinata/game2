#pragma once

class GenerateShadowMapShader
{
public:
	GenerateShadowMapShader() {}
	~GenerateShadowMapShader() {}

	void SetToDevice();

	void DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials);

	void DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld = Math::Matrix::Identity);

	bool Init();

	void Release();

private:
	ID3D11VertexShader* m_VS = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11PixelShader* m_PS = nullptr;

	struct cbObject
	{
		Math::Matrix mWorld;

	};
	KdConstantBuffer<cbObject> m_cb0;
};