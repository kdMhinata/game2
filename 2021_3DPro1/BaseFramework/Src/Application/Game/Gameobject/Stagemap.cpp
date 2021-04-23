#include "Stagemap.h"

StageMap::StageMap()
{
}

StageMap::~StageMap()
{
}

void StageMap::Init()
{
	m_stage.Load("Data/Models/Stage/Stagemap.gltf");
}

void StageMap::Updata()
{
}

void StageMap::Draw()
{
	SHADER->m_effectShader.DrawModel(m_stage);
}

void StageMap::Release()
{
}
