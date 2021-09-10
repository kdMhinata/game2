#pragma once

#include"GameObject.h"

class Arrow : public GameObject
{
public:
	Arrow() {}
	virtual ~Arrow() override { Release(); }
	
	void Init() override;
	void Update() override;

	void DrawEffect() override;

	void SetTarget(const std::shared_ptr<const GameObject> spTarget) { m_wpTarget = spTarget; }

	void SetWorldMatrix(const Math::Matrix& world) { m_mWorld = world; }

private:
	
	void Release();

	void UpdateRotate();
	void RotateByGuide();	// 誘導回転する

	void UpdateCollition();	// 当たり判定の更新
	
	std::weak_ptr<const GameObject>		m_wpTarget;

	float TurnAng = 4.0f;	// 1フレーム回転角度

	int		m_lifeSpan = 0;		// 生存時間

	bool	m_isStabbed = false;

	Math::Vector3		m_prevPos;	// 1フレーム前の座標格納用

	KdTrailPolygon m_trail;
};
