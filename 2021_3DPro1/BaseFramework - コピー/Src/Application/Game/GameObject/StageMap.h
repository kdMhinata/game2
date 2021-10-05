#pragma once

#include"GameObject.h"

class StageMap : public GameObject
{
public:
	StageMap() {}
	~StageMap() { Release(); }

	void Init() override;
	void Draw2D()override;
	void Draw() override;

	classID GetClassID() const override { return eStage; }

private:
	void Release();		// ���

	std::shared_ptr<KdTexture> m_spSampleTex;
};
