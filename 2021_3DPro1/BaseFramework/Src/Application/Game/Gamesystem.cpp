#include "Gamesystem.h"
#include "Gameobject/Stagemap.h"
#include "Gameobject/Player.h"

void GameSystem::Init()
{
	m_sky.Load("Data/Models/Sky/Sky.gltf");
	m_cube.Load("Data/Models/Cube/cube2.gltf");

	m_camera.SetProjectionMatrix(60.0f);
	
	//空の拡縮
	DirectX::SimpleMath::Matrix scale;
	scale = scale.CreateScale(30);
	m_skyMat = scale * m_skyMat;
	
	m_pStage = new StageMap();
	m_pStage->Init();
	m_pPlayer = new Player();
	m_pPlayer->Init();
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
	DirectX::SimpleMath::Matrix cameraPos;
	cameraPos = cameraPos.CreateTranslation({0.0f,0.0f,0.0f});
	m_camera.SetCameraMatrix(cameraPos);

	if (m_pStage)
		m_pStage->Update();
	if (m_pPlayer)
		m_pPlayer->Update();
}

void GameSystem::Draw()
{	
	m_pPlayer->GetCamera().SetToShader();
	//m_camera.SetToShader();
	SHADER->m_effectShader.SetToDevice();
	
	SHADER->m_effectShader.DrawModel(m_sky,m_skyMat);

	SHADER->m_effectShader.DrawModel(m_cube, m_cubeMat);
	if (m_pStage)
		m_pStage->Draw();
	if (m_pPlayer)
		m_pPlayer->Draw();

}

void GameSystem::Release()
{
	OutputDebugStringA("解放関数\n");
	if (m_pStage)
	{
		delete(m_pStage);
		m_pStage = nullptr;
	}

	if (m_pPlayer)
	{
		delete(m_pPlayer);
			m_pPlayer = nullptr;
	}

	bool GameObject::CheckCOllisionBump(const SphereInfo & info, BumpResult & result)
	{
		Math::Vector3 betweenVec = info.m_pos - m_mWorld.Translation();

		float disranceSqr = betweenVec.LengthSquard();

		float hitRadius = m_radius + info.m_radius;

	}

}
