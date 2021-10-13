#include "GenerateShadowMap.h"

void GenerateShadowMapShader::SetToDevice()
{
	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();
	if (!DevCon) { return; }

	//頂点シェーダーをセット
	DevCon->VSSetShader(m_VS,0,0);

	//頂点データのレイアウトをセット
	DevCon->IASetInputLayout(m_inputLayout);

	//ピクセルシェーダーをセット
	DevCon->PSSetShader(m_PS,0,0);

	//14番と15番はシステムによって使用されている
	DevCon->VSSetConstantBuffers(0,1,m_cb0.GetAddress());
}
void GenerateShadowMapShader::DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr)return;

	// 定数バッファ書き込み
	m_cb0.Write();

	// メッシュ情報をセット
	mesh->SetToDevice();

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルセット
		const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];
/*
		//-----------------------
		// マテリアル情報を定数バッファへ書き込む
		//-----------------------
		m_cb1_Material.Work().BaseColor = material.BaseColor;
		m_cb1_Material.Write();

		//-----------------------
		// テクスチャセット
		//-----------------------

		ID3D11ShaderResourceView* srvs[1] = {};
		// BaseColor
		srvs[0] = material.BaseColorTex->WorkSRView();
		// セット
		D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
		*/

		//-----------------------
		// サブセット描画
		//-----------------------
		mesh->DrawSubset(subi);
	}
}
void GenerateShadowMapShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	// 全メッシュノードを描画
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = rModel.GetNodes()[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// 行列セット
		m_cb0.Work().mWorld=rWorkNode.m_worldTransform * mWorld;
		m_cb0.Write();

		// 描画
		DrawMesh(spMesh.get(), data->GetMaterials());
	}
}
bool GenerateShadowMapShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "GenerateShadowMap_VS.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS)))
		{
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 頂点入力レイアウト作成
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],			// 入力エレメント先頭アドレス
			layout.size(),		// 入力エレメント数
			&compiledBuffer[0],				// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),			// 上記のバッファサイズ
			&m_inputLayout))					// 
			)
		{
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "GenerateShadowMap_PS.inc"

	// ピクセルシェーダー作成
		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}
	m_cb0.Create();

	return true;
}

void GenerateShadowMapShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_inputLayout);
	KdSafeRelease(m_PS);

	m_cb0.Release();
}
