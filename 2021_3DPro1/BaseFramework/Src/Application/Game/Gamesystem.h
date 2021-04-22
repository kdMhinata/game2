#pragma once
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

		KdCamera	m_camera;
};