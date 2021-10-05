#include "Lift.h"
#include <Src/Application/main.h>

void Lift::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/Lift/Lift.gltf"));
	
	m_speed = 0.01f;
}

void Lift::SetSpeedByTime(float time)
{
	m_speed = (2.0f) / (time * APP.m_maxFps);
}

void Lift::Update()
{
	Math::Vector3 moveLine = m_endPos - m_startPos;
	Math::Vector3 nowPos = m_startPos + moveLine * (sinf(m_progress) * 0.5f + 0.5f);

	SetPos(nowPos);

	m_progress += m_speed;
}

void Lift::SetRoute(const Math::Vector3& start, const Math::Vector3& end)
{
	m_startPos = start;
	m_endPos = end;
}
