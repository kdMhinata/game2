#include "Gamesystem.h"

void GameSystem::Init()
{
	m_sky.Load("Data/Models/Sky/Sky.gltf");
	m_cube.Load("Data/Models/Cube/cube.gltf");
	m_camera.SetProjectionMatrix(60.0f);
	DirectX::SimpleMath::Matrix rotation;
	rotation = rotation.CreateRotationY(1/*コンバートラディアン*/);
	DirectX::SimpleMath::Matrix& cameraMat = m_camera.WorkCamera();
	cameraMat *= rotation;
	
}

void GameSystem::Update()
{
	
	OutputDebugStringA("更新関数\n");

	//カメラ　上下左右処理(テスト)
	DirectX::SimpleMath::Matrix initPos;
	initPos = initPos.CreateTranslation({ test2*0.01f,test*0.01f, 0.0f });
	m_camera.SetCameraMatrix(initPos);

	if (GetAsyncKeyState(VK_DOWN))
	{
		test -= 1;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		test +=1;
	}
	
	if (GetAsyncKeyState(VK_RIGHT))
	{
		test2 += 1;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		test2 -= 1;
	}

}

void GameSystem::Draw()
{
	m_camera.SetToShader();
	SHADER->m_effectShader.SetToDevice();
	
	SHADER->m_effectShader.DrawModel(m_sky);

	SHADER->m_effectShader.DrawModel(m_cube);

}

void GameSystem::Release()
{
	OutputDebugStringA("解放関数\n");
}
