﻿#include"Gamesystem.h"
#include"GameObject/Stagemap.h"
#include"../main.h";

void GameSystem::Init()
{
	m_sky.SetModel(m_resourceFactory.GetModelData("Data/Models/Sky/Sky.gltf"));
	
	// スカイスフィア拡大行列
	m_skyMat = m_skyMat.CreateScale(50.0f);
}

void GameSystem::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		if (MessageBoxA(APP.m_window.GetWndHandle(), "本当にゲームを終了しますか？",
			"確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
		{
			APP.End();
		}
	}

	// スカイスフィア回転
	DirectX::SimpleMath::Matrix rotSky;
	rotSky = rotSky.CreateRotationY(DirectX::XMConvertToRadians(1.0f * 0.05f));

	// スカイスフィア行列合成
	m_skyMat = rotSky * m_skyMat;

	// objectの更新(範囲for文は参照必須)
	for (std::shared_ptr<GameObject>& spObject : m_spObjects)
	{
		spObject->Update();
	}

	// GameObjectの寿命が尽きたらリストから除去(メモリ解放)
	auto objectItr = m_spObjects.begin();
	
	// 配列の最後の次の配列外まで
	while (objectItr != m_spObjects.end())
	{
		if (!(*objectItr)->IsAlive())
		{
			objectItr->reset(/*引数にポインタを入れて新しくこっちを見さす*/);

			// 消したイテレータを受け取る
			objectItr = m_spObjects.erase(objectItr);

			continue;
		}

		++objectItr;
	}


	OutputDebugStringA("更新関数\n");
}

void GameSystem::Draw()
{
	if (m_spCamera)
	{
		m_spCamera->SetToShader();
	}

	SHADER->m_effectShader.SetToDevice();	// 描画前に必要

	SHADER->m_effectShader.DrawModel(m_sky, m_skyMat);

	// 陰影をつける
	SHADER->m_standardShader.SetToDevice();

	SHADER->m_standardShader.DrawModel(m_cube, m_cubeMat);


	for (std::shared_ptr<GameObject>& spObject : m_spObjects)
	{
		spObject->Draw();
	}
}

void GameSystem::Release()
{
	m_spObjects.clear();
}
