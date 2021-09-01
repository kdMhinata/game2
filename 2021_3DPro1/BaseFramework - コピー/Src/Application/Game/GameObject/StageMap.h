#pragma once

#include"GameObject.h"

class StageMap : public GameObject
{
public:
	StageMap() {}
	~StageMap() { Release(); }

	void Init() override;
	void Draw2D()override;

	classID GetClassID() const override { return eStage; }

private:
	void Release();		// ‰ð•ú

	std::shared_ptr<KdTexture> m_spSampleTex;
};
