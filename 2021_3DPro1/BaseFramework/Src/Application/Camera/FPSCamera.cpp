#include "FPSCamera.h"

const POINT FPSCamera::s_fixMousePos = { 640,340 };

FPSCamera::FPSCamera()
{

};
FPSCamera::~FPSCamera()
{

}

void FPSCamera::Update()
{
	//マウスの移動量を取得する処理
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - s_fixMousePos.x;
	mouseMove.y = nowPos.y - s_fixMousePos.y;

	SetCursorPos(s_fixMousePos.x,s_fixMousePos.y);

	//カメラを回転させる処理
	m_degAngle.x += mouseMove.y * 0.25f;
	m_degANgle.y += mouseMove.x * 0.25f;

	m_degAngle.x = std::clamp(m_degAngle.x, -75.0f, 90.0f);
}
