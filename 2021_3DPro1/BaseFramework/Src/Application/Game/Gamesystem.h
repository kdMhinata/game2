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
		KdModelWork m_cube;

		KdCamera	m_camera;
};