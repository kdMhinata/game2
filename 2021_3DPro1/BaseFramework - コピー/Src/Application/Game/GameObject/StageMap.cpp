#include"StageMap.h"

void StageMap::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/StageMap/StageMap.gltf"));

	m_spSampleTex = GameResourceFactory.GetTexture("Data/Textures/minimap.png");
}

void StageMap::Draw2D()
{
	if (!m_spSampleTex) { return; }
	
	//ビューポートを利用して解像度を得る
	Math::Viewport vp;
	D3D.GetViewport(vp);

	int posX = -(vp.width * 0.5f) + (m_spSampleTex.get()->GetWidth() * 0.5f);
	int posY= (vp.height * 0.5f) - (m_spSampleTex.get()->GetHeight() * 0.5f);

	SHADER->m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER->m_spriteShader.DrawTex(m_spSampleTex.get(),posX,posY);
}

void StageMap::Release()
{
}
