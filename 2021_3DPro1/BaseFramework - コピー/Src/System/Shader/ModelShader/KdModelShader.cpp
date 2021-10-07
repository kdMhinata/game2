#include "System/KdSystem.h"

#include "KdModelShader.h"

bool KdModelShader::Init()
{
    //-------------------------------------
    // 頂点シェーダ
    //-------------------------------------
    {
        // コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdModelShader_VS.inc"

        // 頂点シェーダー作成
        if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
            assert(0 && "頂点シェーダー作成失敗");
            Release();
            return false;
        }

        // １頂点の詳細な情報
        std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	   0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        // 頂点入力レイアウト作成
        if (FAILED(D3D.WorkDev()->CreateInputLayout(
            &layout[0],			// 入力エレメント先頭アドレス
            layout.size(),		      // 入力エレメント数
            &compiledBuffer[0],         // 頂点バッファのバイナリデータ
            sizeof(compiledBuffer),     // 上記のバッファサイズ
            &m_inputLayout))	     // 
            ) {
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
#include "KdModelShader_PS.inc"

        if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
            assert(0 && "ピクセルシェーダー作成失敗");
            Release();
            return false;
        }
    }

    return true;
}

void KdModelShader::Release()
{
    KdSafeRelease(m_VS);
    KdSafeRelease(m_PS);
    KdSafeRelease(m_inputLayout);
}

void KdModelShader::SetToDevice()
{
    // 頂点シェーダをセット
    D3D.WorkDevContext()->VSSetShader(m_VS, 0, 0);
    // 頂点レイアウトをセット
    D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);

    // ピクセルシェーダをセット
    D3D.WorkDevContext()->PSSetShader(m_PS, 0, 0);
}

void KdModelShader::DrawMesh(const KdMesh* mesh, const std::vector<KdMaterial>& materials)
{
    if (mesh == nullptr)return;

    // 頂点シェーダをセット
    D3D.WorkDevContext()->VSSetShader(m_VS, 0, 0);
    // 頂点レイアウトをセット
    D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);

    // メッシュ情報をセット
    mesh->SetToDevice();

    // 全サブセット
    for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
    {
        // 面が１枚も無い場合はスキップ
        if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

        //-----------------------
        // サブセット描画
        //-----------------------
        mesh->DrawSubset(subi);
    }
}

void KdModelShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
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

        // 描画
        DrawMesh(spMesh.get(), data->GetMaterials());
    }
}
