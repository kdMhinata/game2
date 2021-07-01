#include "Stagemap.h"

StageMap::StageMap()
{
}

StageMap::~StageMap()
{
}

void StageMap::Init()
{

	m_modelWork.Load("Data/Models/Stage/StageMap.gltf");
}

void StageMap::Update()
{
}

void StageMap::Draw()
{
	SHADER->m_effectShader.DrawModel(m_modelWork);
}

void StageMap::Release()
{
}
