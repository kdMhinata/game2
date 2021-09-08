﻿#pragma once

// 前方宣言
class GameObject;

class GameSystem
{
public:

	void TitleInit();
	void GameInit();
	void ResultInit();

	void Init();
	void Update();
	void Draw();

	void AddObject(std::shared_ptr<GameObject> spObject)
	{
		if (spObject){ m_spObjects.push_back(spObject);}
	}

	// 参照(&)は必須
	const std::list<std::shared_ptr<GameObject>>& GetObjects() { return m_spObjects; }

	void SetCamera(std::shared_ptr<KdCamera> spCamera) { m_spCamera = spCamera; }

	ResourceFactory& WorkResourceFactory() { return m_resourceFactory; }

	const std::shared_ptr<KdCamera> GetCamera() const;
private:
	void Release();		// 解放

	KdModelWork m_sky;	// スカイスフィア

	DirectX::SimpleMath::Matrix m_cubeMat;	// キューブのワールド行列
	DirectX::SimpleMath::Matrix m_skyMat;	// キューブのワールド行列

	std::list<std::shared_ptr<GameObject>> m_spObjects;

	std::shared_ptr<KdCamera> m_spCamera = nullptr;

	ResourceFactory m_resourceFactory;

	//サウンドエンジン
	std::unique_ptr<DirectX::AudioEngine> m_audioEng = nullptr;

	//再生サウンドの元データ
	std::vector<std::unique_ptr<DirectX::SoundEffect>> m_soundEffect;

	//サウンドの再生インスタンス
	std::vector < std::unique_ptr<DirectX::SoundEffectInstance>> m_instance;

	// シングルトンパターン化
public:

	static GameSystem& GetInstance()
	{
		static GameSystem instance;

		return instance;
	}


private:
	GameSystem() {}	
	~GameSystem() { Release(); }
};

#define GameInstance GameSystem::GetInstance() 

#define GameResourceFactory GameSystem::GetInstance().WorkResourceFactory()