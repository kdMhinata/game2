#pragma once
class GameSystem
{
public:
	GameSystem() {}

	~GameSystem() { Release(); }

	void Init();
	void Update();
	void Draw();
	float test=0; //テスト用変数
	float test2 = 0; //テスト用変数2

private:
		void Release();

		KdModelWork m_sky;
		KdModelWork m_cube;
		DirectX::SimpleMath::Matrix m_cubeMat;

		KdCamera	m_camera;
};