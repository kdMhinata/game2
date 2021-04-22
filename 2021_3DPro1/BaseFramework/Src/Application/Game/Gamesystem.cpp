#include "Gamesystem.h"

void GameSystem::Init()
{
	m_sky.Load("Data/Models/Sky/Sky.gltf");
	m_camera.SetProjectionMatrix(60.0f);
}

void GameSystem::Update()
{
	
	OutputDebugStringA("更新関数\n");
}

void GameSystem::Draw()
{
	m_camera.SetToShader();
	SHADER->m_effectShader.SetToDevice();
	
	SHADER->m_effectShader.DrawModel(m_sky);
}

void GameSystem::Release()
{
	OutputDebugStringA("解放関数\n");
}
