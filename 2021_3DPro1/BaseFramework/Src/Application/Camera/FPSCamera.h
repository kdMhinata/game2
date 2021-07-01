#pragma once

class FPSCamera : public KdCamera
{
public:
	FPSCamera() {}
	~FPSCamera() {}
	static const POINT s_fixMousePos;

	void Update();
private:

	KdCamera m_camera;
};