#include "Gamesystem.h"
#include "Gameobject/Stagemap.h"

void GameSystem::Init()
{
	m_sky.Load("Data/Models/Sky/Sky.gltf");
	m_cube.Load("Data/Models/Cube/cube2.gltf");

	m_camera.SetProjectionMatrix(60.0f,2000);
	
	//空の拡縮
	DirectX::SimpleMath::Matrix scale;
	scale = scale.CreateScale(30);
	m_skyMat = scale * m_skyMat;
	
	StageMap* Init();
}

void GameSystem::Update()
{
	
	OutputDebugStringA("更新関数\n");

	/*DirectX::SimpleMath::Matrix rotation;
	rotation = rotation.CreateRotationY(DirectX::XMConvertToRadians(1));
	DirectX::SimpleMath::Matrix& cameraMat = m_camera.WorkCamera();
	cameraMat = cameraMat * rotation;*/
	static DirectX::SimpleMath::Matrix rotation;
	static DirectX::SimpleMath::Matrix translation;

	
	rotation *= rotation.CreateRotationY(DirectX::XMConvertToRadians(2.0f));

	//授業指定 箱を奥に
	if (GetAsyncKeyState('Z'))
	{
		DirectX::SimpleMath::Vector3 v_Move( 0.0f,0.0f,0.01f );
		v_Move=v_Move.Transform(v_Move, rotation);
		translation *= translation.CreateTranslation(v_Move);
	}
	m_cubeMat = rotation * translation;


	//カメラ　上下左右処理(テスト)
	DirectX::SimpleMath::Matrix cmeraPos;
	cmeraPos = cmeraPos.CreateTranslation({ test2 * 0.1f,test * 0.1f, test3 * 0.1f });
	m_camera.SetCameraMatrix(cmeraPos);

	if (GetAsyncKeyState('S'))
	{
		test -= 1;
	}

	if (GetAsyncKeyState('W'))
	{
		test += 1;
	}

	if (GetAsyncKeyState('D'))
	{
		test2 += 1;
	}

	if (GetAsyncKeyState('A'))
	{
		test2 -= 1;
	}

	if (GetAsyncKeyState(VK_XBUTTON1))
	{
		test3 -= 1;
	}
	else if (GetAsyncKeyState(VK_XBUTTON2))
	{
		test3 += 1;
	}
}

void GameSystem::Draw()
{
	m_camera.SetToShader();
	SHADER->m_effectShader.SetToDevice();
	
	SHADER->m_effectShader.DrawModel(m_sky,m_skyMat);

	SHADER->m_effectShader.DrawModel(m_cube,m_cubeMat);
	StageMap* Draw();

}

void GameSystem::Release()
{
	OutputDebugStringA("解放関数\n");
}
