#pragma once

//モデル描画用シェーダー

class KdModelShader
{
public :
	bool Init();
	void Release();

	//今からこのシェーダーを使うので設定
	void SetToDevice();

	void DrawModel(const KdModelWork& rModel,
					const Math::Matrix& mWorld=Math::Matrix::Identity);

	//モデルから取り出したメッシュ(ノード)を描画する
	void DrawMesh(const KdMesh* mesh,
					const std::vector<KdMaterial>& materials);
private :
	//シェーダーを読み込んで確保しておく場所
	ID3D11VertexShader* m_VS = nullptr;

	//頂点シェーダーが受け取る頂点情報
	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11PixelShader* m_PS = nullptr;
};