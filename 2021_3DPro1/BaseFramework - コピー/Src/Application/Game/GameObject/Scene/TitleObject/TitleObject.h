#pragma once

#include "Src/Application/Game/GameObject/GameObject.h"
class TitleObject : public GameObject
{
public:
	TitleObject() {}
	~TitleObject() { Release(); }

	void Init() override; 
	void Update() override;
	void Draw2D()override;

private:
	void Release();		// ���

	std::shared_ptr<KdTexture> m_spTitleTex;
};
