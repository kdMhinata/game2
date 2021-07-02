#pragma once

#include"GameObject.h"

class TPSCamera;

class Player : public GameObject
{
public:
	Player();
	virtual ~Player() override;

	void Init() override;
	void Update() override;

	const std::shared_ptr<const TPSCamera> GetCamera() const { return m_spCamera; }	// �Q�Ɨp
	std::shared_ptr<TPSCamera> WorkCamera() { return m_spCamera; }				// �ύX�p

	const Math::Vector3 GetPos() const override{ return m_worldPos; }

	classID GetClassID() const override { return ePlayer; }

private:
	void Release();		// ���

	void UpdateMove(Math::Vector3& dstMove);
	void UpdateRotate(const Math::Vector3& srcMove);
	void UpdateMatrix();

	void UpdateCollition();		// �����蔻��̍X�V

	void ShotArrow();

	Math::Vector3	m_worldPos;		// ���[���h�s�����邽�߂̍��W
	Math::Vector3	m_worldRot;		// ���[���h�s�����邽�߂̉�]

	std::shared_ptr<TPSCamera>		m_spCamera;
	
	bool m_canShot = true;

	float m_gravity = 0.0f;
};

