#include"Player.h"
//#include"../Camera/FPSCamera.h"
#include"../Camera/TPSCamera.h"
#include"Arrow.h"
#include"../GameSystem.h"
#include "Src/Application/Utility/DebugObject/DebugObject.h"

const float Player::s_limitOfStepHeight = 0.5f;

Player::Player()
{
}

Player::~Player()
{
	Release();
}

void Player::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/SkinMeshMan/SkinMeshMan.gltf"));

	m_spCamera = std::make_shared<TPSCamera>();

	GameSystem::GetInstance().SetCamera(m_spCamera);

	m_spCamera->Init();

	m_spCamera->SetProjectionMatrix(60.0f, 3000.0f);	// ����p�̐ݒ�i���E��60�x��120�x�j,�ő�`�拗��(�Z���قǔ��肪���m�ɂȂ�)

	// �J�����̒����_����5m�����
	m_spCamera->SetLocalPos(Math::Vector3(0.0f, 0.0f, -5.0f));

	// �L�����N�^�[���璍���_�ւ̃��[�J�����W�����3m�グ��
	m_spCamera->SetLocalGazePos(Math::Vector3(0.0f, 3.0f, 0.0f));

	// �J�����̐����p�x
	m_spCamera->SetClampAngle(-75.0f,90.0f);

	// �J�����̈ړ��X�s�[�h
	m_spCamera->SetRotationSpeed(0.25);

	m_radius = 0.5f;

	m_animator.SetAnimation(m_modelWork.GetData()->GetAnimation("Walk"));

	m_spActionState = std::make_shared<ActionWait>();

	//�f�o�b�OObject�̃C���X�^���X��
	m_spDebugObject = std::make_shared<DebugObject>();
		
}

// �X�V����
void Player::Update()
{
	m_gravity += 0.01f;
	m_prevPos = GetPos();

	if(m_spActionState)
	m_spActionState->Update(*this);

	if (GetAsyncKeyState(VK_SPACE)) 
	{
		if (m_canJump)
		{
			m_gravity = -0.5f;
			m_canJump = false;
		}
	}

	m_worldPos.y -= m_gravity;

	// �����蔻��X�V
	UpdateCollition();

	// �s��̍X�V(�ŏI�I�ȍ��W���m�肵�Ă���)
	UpdateMatrix();

	ShotArrow();

	// �J�������\������s��̍������ʂ��Z�b�g
	if (m_spCamera)
	{
		m_spCamera->Update();

		Math::Matrix trans = Math::Matrix::CreateTranslation(m_worldPos);

		// �v���C���[�̐�΍s��̃Z�b�g
		m_spCamera->SetCameraMatrix(trans);
	}

	m_animator.AdvanceTime(m_modelWork.WorkNodes());

	m_modelWork.CalcNodeMatrices();

	UpdateDebug();
}

void Player::Release()
{
	m_spCamera.reset();
}

// �ړ��̍X�V����
void Player::UpdateMove(Math::Vector3& dstMove)
{
	// �L�����̈ړ�����
	float moveSpd = 0.05f;

	Math::Vector3 moveVec;

	if (GetAsyncKeyState('W') & 0x8000) { moveVec.z += 1.0f; }	// �O�ړ�
	if (GetAsyncKeyState('S') & 0x8000) { moveVec.z -= 1.0f; }	// ���ړ�
	if (GetAsyncKeyState('A') & 0x8000) { moveVec.x -= 1.0f; }	// ���ړ�
	if (GetAsyncKeyState('D') & 0x8000) { moveVec.x += 1.0f; }	// �E�ړ�

	// �΂߈ړ��ɂ�������␳
	moveVec.Normalize();
	moveVec *= moveSpd;

	if (m_spCamera)
	{
		// �i�s�������J������]���������ďC������
		moveVec = moveVec.TransformNormal(moveVec, m_spCamera->GetRotationYMatrix());
	}

	// ���W�m��
	m_worldPos.x += moveVec.x;
	m_worldPos.z += moveVec.z;

	dstMove = moveVec;
}

// ��]�̍X�V����
void Player::UpdateRotate(const Math::Vector3& srcMove)
{
	if (!m_spCamera) { return;}

	// �x�N�g���̒������[����������
	if (srcMove.LengthSquared() == 0.0f) { return; }

	// �L�����̐��ʕ����x�N�g���F�o���n
	Math::Vector3 nowDir = m_mWorld.Backward();
	
	// �ړ������̃x�N�g���F�ڕW�n
	Math::Vector3 targetDir = srcMove;

	nowDir.Normalize();
	targetDir.Normalize();

	// ���ꂼ���Degree�p�x�����߂�
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// �Q�̊Ԃ̊p�x�����߂�
	float rotateAng = targetAng - nowAng;

	// ��]�̕␳
	if (rotateAng > 180) 
	{
		rotateAng -= 360; 
	}
	else if (rotateAng < -180)
	{
		rotateAng += 360;
	}

	// ��]�ʑ��
	rotateAng = std::clamp(rotateAng, -8.0f, 8.0f);
	
	m_worldRot.y += rotateAng;
}

// �s��X�V
void Player::UpdateMatrix()
{
	// ���[���h�s����쐬
	Math::Matrix trans;
	trans = Math::Matrix::CreateTranslation(m_worldPos);
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = rotation * trans;
}

void Player::UpdateDebug()
{
	if (!m_spDebugObject) { return; }
	
	//�o�o�O���C����o�^����
	Math::Vector3 localEndPos = GetPos();
	localEndPos.y += 10.0f;
	m_spDebugObject->AddDebugLine(GetPos(), localEndPos);
	

	/*//�f�o�b�O�X�t�B�A��`�悷��
	Math::Vector3 localSpherePos = GetPos() + m_bumpSphereInfo, m_pos;
	m_spDebugObject->AddDebugSphere(localSpherePos,m_bumpSphereInfo)
	*/
}

// �����蔻��̍X�V
void Player::UpdateCollition()
{
	for (const std::shared_ptr<GameObject>& spObj : GameSystem::GetInstance().GetObjects())
	{
		if (spObj->GetClassID() != GameObject::eEnemy) { continue; }

		SphereInfo info(GetPos(),m_radius);

		BumpResult result;

		// ����̔���֐��𗘗p����
		if (spObj->CheckCollisionBump(info, result))
		{
			m_worldPos += result.m_pushVec;
		}
	}

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage) { continue; }

		Math::Vector3 rayPos = m_prevPos;
		//�@�����Ĉړ��ł���n�ʂ̌��E�̒i��
		rayPos.y += s_limitOfStepHeight;

		RayInfo rayInfo(rayPos, Math::Vector3(0.0f, -1.0f, 0.0f), m_gravity+s_limitOfStepHeight);

		BumpResult bumpResult;

		spObject->CheckCollisionBump(rayInfo, bumpResult);

		if(bumpResult.m_isHit)
		{
			m_worldPos += bumpResult.m_pushVec;
			m_gravity = 0.0f;
			m_canJump = true;
		}
	}

}

void Player::ShotArrow()
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (!m_canShot) { return; }

		// ����C���X�^���X��
		std::shared_ptr<Arrow> pArrow = std::make_shared<Arrow>();

		pArrow->Init();

		const std::list<std::shared_ptr<GameObject>>& objects =
			GameSystem::GetInstance().GetObjects();

		// �Q�Ƃ�t���邱�Ƃ�shared_ptr�𕡐����Ȃ�
		for (const std::shared_ptr<GameObject>& spObject : objects)
		{
			if (spObject->GetClassID() == GameObject::eEnemy)
			{
				// Enemy�̂��̂���������Z�b�g
				pArrow->SetTarget(spObject);
				
				break;
			}
		}

		// �v���C���[�̓������𔭎�
		const KdModelWork::Node* pNode = m_modelWork.FindNode("Head");

		// �m�[�h������Γ�����
		if (pNode)
		{	// m_worldTransform �c ���f���̌��_����̃��[�J���s��
			pArrow->SetWorldMatrix(m_mWorld * pNode->m_worldTransform);
		}
		else
		{
			pArrow->SetWorldMatrix(m_mWorld);
		}

		GameSystem::GetInstance().AddObject(pArrow);

		m_canShot = false;
	}
	else { m_canShot = true; }
}

void Player::ActionWait::Update(Player& owner)
{
	//�ړ��L�[�������ꂽ�H
	if (GetAsyncKeyState('W')) { owner.m_spActionState = std::make_shared<ActionMove>(); }

	//�W�����v�L�[�����ꂽ�H

}


void Player::ActionMove::Update(Player& owner)
{
	//�ړ��L�[��������Ă��Ȃ��H
	if (!GetAsyncKeyState('W')) { owner.m_spActionState = std::make_shared<ActionWait>(); }

	Math::Vector3 vMove;

	// �ړ��̍X�V����
	owner.UpdateMove(vMove);

	// ��]�̍X�V����
	owner.UpdateRotate(vMove);

	//vmoven�̒�����0��������ړ����ĂȂ��Ƃ݂Ȃ�
	if (vMove.LengthSquared() == 0)
	{
		owner.ChangeWait();
	}
}
