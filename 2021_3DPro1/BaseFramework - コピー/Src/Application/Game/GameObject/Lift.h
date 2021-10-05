#pragma once

#include "GameObject.h"
#include "StageMap.h"

class Lift : public StageMap
{
public:
	void Init() override;
	void SetSpeedByTime(float time);
	void Update() override;

	void SetRoute(const Math::Vector3& start,
	const Math::Vector3& end);

	void SetSpeed(float speed) { m_speed = speed; }

private:
	void Release();
	Math::Vector3 m_startPos;
	Math::Vector3 m_endPos;

	float m_speed = 0.0f;
	float m_progress = 0.0f;
	bool m_isReverse = false;
};