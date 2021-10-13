#pragma once

#include "GameObject.h"

class Effect2D :public GameObject
{
public:
	Effect2D() {}
	~Effect2D() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawEffect() override;

	void SetTexture(const std::shared_ptr<KdTexture> spTex
		, float w = 1.0f, float h = 1.0f, Math::Color col = kWhiteColor);
	
	void SetAnimation(int splitX, int splitY,float speed=1.0f,bool isLoop=false);

	void SetLifeSpan(int lifeSpan) { m_lifeSpan = lifeSpan; }

	//void CreateExplode(); //�����I�ɂ͂�����O���t�@�C����������������
	//void CreateEffect(FileObject file);

	const KdSquarePolygon GetPolyData() const { return m_poly; }
private:

	KdSquarePolygon m_poly;

	float m_animSpd = 0.0f; //�A�j���[�V�����̑��x

	bool  m_isLoop = false;

	int m_lifeSpan = -1;

	float m_angleZ = 0.0f; //�ċx�ݏ��ۑ�@�G�t�F�N�g�`�掞�ɂ��炵�Ă�����ۂ�����
};