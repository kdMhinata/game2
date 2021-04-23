#include "Stagemap.h"

StageMap::StageMap()
{
}

StageMap::~StageMap()
{
}

void StageMap::Init()
{

	m_modelWork.Load("Data/Models/Stage/StageMapx.gltf");
}

void StageMap::Updata()
{
}

void StageMap::Draw()
{
	SHADER->m_effectShader.DrawModel(m_modelWork);
}

void StageMap::Release()
{
}
