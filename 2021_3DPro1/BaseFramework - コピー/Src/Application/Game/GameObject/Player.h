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
	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
		m_prevPos = pos;
	}

	classID GetClassID() const override { return ePlayer; }

private:

	//	�����Ē���������E�̍���
	static const float s_limitOfStepHeight;

	void Release();		// ���

	void UpdateMove(Math::Vector3& dstMove);
	void UpdateRotate(const Math::Vector3& srcMove);
	void UpdateMatrix();

	void UpdateDebug();

	void UpdateCollition();		// �����蔻��̍X�V

	void ShotArrow();

	Math::Vector3	m_worldPos;		// ���[���h�s�����邽�߂̍��W
	Math::Vector3	m_worldRot;		// ���[���h�s�����邽�߂̉�]
	Math::Vector3   m_prevPos;		// 1�t���[���O�̍��W


	std::shared_ptr<TPSCamera>		m_spCamera;

	KdAnimator m_animator;
	
	bool m_canShot = true;
	bool m_canJump = true;

	void ChangeWait()
	{
		m_spActionState = std::make_shared<ActionWait>();
	}
	void ChangeMove();
	void ChangeJump();
	float m_gravity = 0.0f;

	class BaseAction
	{
	public:
		virtual void Update(Player& owner) = 0; //�������z�֐�
	};

	class ActionWait : public BaseAction
	{
	public:
		void Update(Player& owner) override;
	};

	class ActionMove : public BaseAction
	{
	public:
		void Update(Player& owner) override;
	};

	std::shared_ptr<BaseAction> m_spActionState =nullptr;

};

