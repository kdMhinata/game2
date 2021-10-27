#pragma once

class GenerateShadowMapShader
{
public:
	GenerateShadowMapShader() {}
	~GenerateShadowMapShader() {}

	void Begin();
	void End();

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

	std::shared_ptr<KdTexture> m_dirLightShadowMap=nullptr;
	std::shared_ptr<KdTexture> m_dirLightZBuffer=nullptr;

	ID3D11RenderTargetView* m_saveRTV = nullptr;
	ID3D11DepthStencilView* m_saveDSV = nullptr;
	D3D11_VIEWPORT			m_saveVP = {};

};