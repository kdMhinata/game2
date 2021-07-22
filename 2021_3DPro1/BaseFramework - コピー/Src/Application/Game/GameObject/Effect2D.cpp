#include "Effect2D.h"

void Effect2D::Init()
{
	//m_poly.SetTexture(	GameResourceFactory.GetTexture("Data/Texture/Explosion.png"));

	m_poly.Init(1.0f, 1.0f, kWhiteColor);
}

void Effect2D::Update()
{
	if (m_poly.GetAnimationSize()<=1) { return; }
	m_poly.Animation(m_animSpd,m_isLoop);

	if (m_poly.GetAnimationSize() <= 1||m_isLoop)
	{
		if (--m_lifeSpan <= 0)
		{
			m_isAlive = false;
		}
	}

	//�A�j���[�V�����������Ȃ�A�j���[�V�����̍X�V�E�I��������s��Ȃ�
	if (m_poly.GetAnimationSize() <= 1) { return; }

	m_poly.Animation(m_animSpd, m_isLoop);

	if (m_poly.IsAnimationEnd())
	{
		m_isAlive = false;
	}
}

void Effect2D::Draw()
{
	SHADER->m_effectShader.DrawSquarePolygon(m_poly, m_mWorld);
}

void Effect2D::DrawEffect()
{
	//�`��p�̍s��
	Math::Matrix mDraw;

	//�s��̊g��=S
	//----------------------------------------------------------
	Math::Vector3 scale;
	scale.x = m_mWorld.Right().Length();
	scale.y = m_mWorld.Up().Length();
	scale.z = m_mWorld.Forward().Length();
	mDraw = Math::Matrix::CreateScale(scale);

	//�s��̉�]=R
	//----------------------------------------------------------
	std::shared_ptr<KdCamera> gameCam = GameInstance.GetCamera();

	if (gameCam)
	{
		//�J�����̋t�s��̍���=���[���h�s��->�J�������猩���s��ɂ���
		Math::Matrix mCamInv = gameCam->GetCameraMatrix();
		mCamInv.Invert();

		//�J�����̖ڂ̑O�Ɏ����Ă���
		mDraw *= mCamInv;
	}

	//�s��̈ړ�=T
	//-------------------------------------------------------------
	mDraw.Translation(m_mWorld.Translation());


	SHADER->m_effectShader.DrawSquarePolygon(m_poly, mDraw);
}
void Effect2D::SetTexture(const std::shared_ptr<KdTexture> spTex, float w, float h, Math::Color col)
{
	if (!spTex) { return; }
	m_poly.SetTexture(spTex);

	m_poly.Init(w, h, col);
}

void Effect2D::SetAnimation(int splitX, int splitY,float speed, bool isLoop)
{
	m_poly.SetAnimationInfo(splitX, splitY);

	m_animSpd = speed;
	m_isLoop = isLoop;
}
