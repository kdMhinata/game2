#pragma once

class GenerateShadowMapShader
{
public:
	GenerateShadowMapShader() {}
	~GenerateShadowMapShader() {}

	bool Init();

	void Release();

private:
	ID3D11VertexShader* m_VS = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11PixelShader* m_PS = nullptr;
};