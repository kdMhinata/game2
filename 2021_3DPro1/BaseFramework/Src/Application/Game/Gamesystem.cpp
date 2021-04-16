#include "Gamesystem.h"

void GameSystem::Init()
{
	m_sky.Load();
}

void GameSystem::Update()
{
	OutputDebugStringA("更新関数\n");
}

void GameSystem::Draw()
{
	OutputDebugStringA("描画関数\n");
}

void GameSystem::Release()
{
	OutputDebugStringA("解放関数\n");
}
