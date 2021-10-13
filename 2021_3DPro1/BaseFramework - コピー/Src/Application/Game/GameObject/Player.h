#pragma once

#include"GameObject.h"

class TPSCamera;
class Effect2D;

class Player : public GameObject
{
public:
	Player();
	virtual ~Player() override;

	void Init() override;
	void Draw2D() override;
	void DrawEffect()override;
	void Update() override;

	//ターゲットObjectの設定
	void SetTraget(std::shared_ptr<const GameObject>pTarget)
	{
		m_wpTarget = pTarget;
	}

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
	std::weak_ptr<GameObject>  m_wpRideObj;
	Math::Matrix m_mLocalFromRide; //乗物からの相対行列

	//	歩いて超えられる限界の高さ
	static const float s_limitOfStepHeight;

	void Release();		// 解放

	void UpdateMove(Math::Vector3& dstMove);
	void UpdateRotate(const Math::Vector3& srcMove);
	void UpdateMatrix();

	void UpdateDebug();
	
	void UpdatePosFromRideobj();
	void UpdateLocalFromRide();

	void UpdateCollition();		// 当たり判定の更新

	void ShotArrow();

	Math::Vector3	m_worldPos;		// ワールド行列を作るための座標
	Math::Vector3	m_worldRot;		// ワールド行列を作るための回転
	Math::Vector3   m_prevPos;		// 1フレーム前の座標
	std::weak_ptr<const GameObject> m_wpTarget;

	std::shared_ptr<TPSCamera>		m_spCamera;
	std::shared_ptr<Effect2D> m_spShadow=nullptr;

	KdAnimator m_animator;

	std::shared_ptr<KdTexture> m_ScopeTex;

	
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

