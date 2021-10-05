#pragma once

#include"GameObject.h"

class Enemy : public GameObject
{
public: 
	Enemy(){}
	virtual ~Enemy() override { Release(); }

	void Init() override;
	void Update() override;

	void DrawEffect() override;

	void SetTarget(std::shared_ptr<const GameObject> spTarget) { m_wpTarget = spTarget; }

	classID GetClassID() const override{ return eEnemy; }

	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
	}

private:

	void Release();

	void UpdateMove();
	void UpdateRotate();

	Math::Vector3	m_worldPos;
	Math::Vector3	m_worldRot;

	std::weak_ptr<const GameObject> m_wpTarget;

	float m_stopDist = 5.0f;

	class TrailNode
	{
	public:
		void Update();
		void Draw();
	private:
		KdTrailPolygon m_trail;
	};

	std::list<std::shared_ptr<TrailNode>> m_trailList;

	KdTrailPolygon m_trail;

	KdModelWork::Node* m_proNodes[3];
};