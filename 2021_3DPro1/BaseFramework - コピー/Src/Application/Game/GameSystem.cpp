#include"GameSystem.h"
#include"GameObject/StageMap.h"
#include"GameObject/Player.h"
#include"GameObject/Enemy.h"
#include"GameObject/Arrow.h"
#include"GameObject/Effect2D.h"
#include"GameObject/Scene/TitleObject/TitleObject.h"

#include"Camera/TPSCamera.h"

#include"../main.h";

void GameSystem::TitleInit()
{//背景のスプライト描画用GameObject
	//ゲームシーンに推移するためのボタンObject
	std::shared_ptr<TitleObject> spTitle = std::make_shared<TitleObject>();
	spTitle->Init();
}


void GameSystem::GameInit()
{
	//前のシーンにあった情報は解放して何もない状態にする
	Release();
	m_sky.SetModel(m_resourceFactory.GetModelData("Data/Models/Sky/Sky.gltf"));

	// スカイスフィア拡大行列
	m_skyMat = m_skyMat.CreateScale(50.0f);


	std::shared_ptr<StageMap> spStage = std::make_shared<StageMap>(); // stageMapのインスタンス化
	spStage->Init();
	AddObject(spStage);

	std::shared_ptr<Player> spPlayer = std::make_shared<Player>();	// プレイヤーのインスタンス化
	spPlayer->Init();
	AddObject(spPlayer);

	std::shared_ptr<Enemy> spEnemy = std::make_shared<Enemy>();
	spEnemy->Init();
	AddObject(spEnemy);
	spEnemy->SetTarget(spPlayer);

	std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();

	spEffect->Init();

	spEffect->SetAnimation(5, 5);

	AddObject(spEffect);

	//AudioEngin初期化
	DirectX::AUDIO_ENGINE_FLAGS eflags =
		DirectX::AudioEngine_EnvironmentalReverb | DirectX::AudioEngine_ReverbUseFilters;

	m_audioEng = std::make_unique<DirectX::AudioEngine>(eflags);
	m_audioEng->SetReverb(DirectX::Reverb_Default);

	//BGMデータファイルのLoad
	//※wstringに変換 const charは受け取ってもらえない
	std::wstring wFilename = sjis_to_wide("Data/Audio/BGM/castle.wav");

	//0番に読み込み
	std::unique_ptr<DirectX::SoundEffect> soundEffect=
		std::make_unique<DirectX::SoundEffect>(m_audioEng.get(),wFilename.c_str() );
	m_soundEffect.push_back(std::move(soundEffect));

	wFilename = sjis_to_wide("Data/Audio/SE/ItemGet.wav");
	//1番に読み込み
	std::make_unique<DirectX::SoundEffect>(m_audioEng.get(), wFilename.c_str());
	m_soundEffect.push_back(std::make_unique<DirectX::SoundEffect>(m_audioEng.get(),wFilename.c_str()));
				
	//サウンド(BGM)再生用Instanceの作成
	if (m_soundEffect[0] != nullptr)
	{
		DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
		std::unique_ptr<DirectX::SoundEffectInstance> instance =
			m_soundEffect[0]->CreateInstance(flags);

		if (instance)
		{
			instance->Play();

			m_instance.push_back(std::move(instance));
		}
	}
}

void GameSystem::ResultInit()
{
	
}

void GameSystem::Init()
{
	//ほんへ
	GameInit();

	//TitleInit();
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

		// キャンセル時元の位置に戻す
		SetCursorPos(FPSCamera::s_fixMousePos.x, FPSCamera::s_fixMousePos.y);
	}

	if (GetAsyncKeyState(VK_TAB))
	{
		//サウンド(BGM)再生用Instanceの作成
		if (m_soundEffect[1] != nullptr)
		{
			DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
			std::unique_ptr<DirectX::SoundEffectInstance> instance =
				m_soundEffect[1]->CreateInstance(flags);

			if (instance)
			{
				instance->Play();

				m_instance.push_back(std::move(instance));
			}
		}
	}

	/*if (m_isRequestChangeScene)
	{
		ChangeScene();
		return;
	}*/

	// キューブ回転行列
	DirectX::SimpleMath::Matrix rot;
	rot = rot.CreateRotationY(DirectX::XMConvertToRadians(1.0f));

	m_cubeMat = rot * m_cubeMat;
	m_cubeMat = m_cubeMat * rot;


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
	//std::list<std::shared_ptr<GameObject>>::iterator objectItr = m_spObjects.begin();
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
}

void GameSystem::Draw()
{
	if (m_spCamera)
	{
		m_spCamera->SetToShader();
	}

	// 陰影をつける
	SHADER->m_standardShader.SetToDevice();

	for (std::shared_ptr<GameObject>& spObject : m_spObjects)
	{
		spObject->Draw();
	}

	SHADER->m_effectShader.SetToDevice();	// 描画前に必要

	SHADER->m_effectShader.DrawModel(m_sky,m_skyMat);

	{
		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteDisable, 0);

		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullNone); // カリングなし

		//透明物の描画
		for (std::shared_ptr<GameObject>& spObject : m_spObjects)
		{
			spObject->DrawEffect();
		}

		D3D.WorkDevContext()->OMSetDepthStencilState(SHADER->m_ds_ZEnable_ZWriteEnable, 0);

		D3D.WorkDevContext()->RSSetState(SHADER->m_rs_CullBack);
	}

	SHADER->m_spriteShader.Begin();

	SHADER->m_spriteShader.End();


}

const std::shared_ptr<KdCamera> GameSystem::GetCamera() const
{
	return m_spCamera;
}

void GameSystem::Release()
{
	m_spObjects.clear();
}
