#pragma once

class FPSCamera:
class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw();

	KdCamera& GetCamera() { return m_camera; }

private:
	void Release();
	
	KdModelWork m_modelWork;

	DirectX::SimpleMath::Matrix m_mWorld;


	DirectX::SimpleMath::Matrix m_mWorld;
	KdCamera m_camera;
	Math::Matrix m_mLocalCamera;
	Math::Matrix m_RotateCamera;
};