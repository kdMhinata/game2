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

	const std::shared_ptr<const TPSCamera> GetCamera() const { return m_spCamera; }	// 参照用
	std::shared_ptr<TPSCamera> WorkCamera() { return m_spCamera; }				// 変更用

	const Math::Vector3 GetPos() const override{ return m_worldPos; }
	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
		m_prevPos = pos;
	}

	classID GetClassID() const override { return ePlayer; }

private:

	//	歩いて超えられる限界の高さ
	static const float s_limitOfStepHeight;

	void Release();		// 解放

	void UpdateMove(Math::Vector3& dstMove);
	void UpdateRotate(const Math::Vector3& srcMove);
	void UpdateMatrix();

	void UpdateDebug();

	void UpdateCollition();		// 当たり判定の更新

	void ShotArrow();

	Math::Vector3	m_worldPos;		// ワールド行列を作るための座標
	Math::Vector3	m_worldRot;		// ワールド行列を作るための回転
	Math::Vector3   m_prevPos;		// 1フレーム前の座標


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
		virtual void Update(Player& owner) = 0; //純粋仮想関数
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

