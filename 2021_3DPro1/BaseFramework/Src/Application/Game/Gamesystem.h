#pragma once

class StageMap;
class Player;

class GameSystem
{
public:
	GameSystem() {}

	~GameSystem() { Release(); }

	void Init();
	void Update();
	void Draw();

private:
		void Release();

		KdModelWork m_sky;
		DirectX::SimpleMath::Matrix m_skyMat;

		KdModelWork m_cube;
		DirectX::SimpleMath::Matrix m_cubeMat;

		KdCamera	m_camera;

		StageMap*	m_pStage= nullptr;
		Player* m_pPlayer = nullptr;
};