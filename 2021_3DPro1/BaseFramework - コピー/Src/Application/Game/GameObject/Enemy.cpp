#include"Enemy.h"
#include"Player.h"

void Enemy::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/Robot/Robocopter.gltf"));
	
	m_radius = 0.5f;

	m_worldPos.x = 2.0f;

		m_trail.SetTexture(GameResourceFactory.GetTexture("Data/Textures/Trail.png"));
		m_trail.SetWidth(0.2f);
		m_trail.SetPattern(KdTrailPolygon::Trail_Pattern::eVertices);

	m_proNodes[0] = m_modelWork.FindWorkNode("Propeller_Top1");
	m_proNodes[1] = m_modelWork.FindWorkNode("Propeller_Top2");
	m_proNodes[2] = m_modelWork.FindWorkNode("Propeller_Top3");
}

void Enemy::Update()
{
	UpdateMove();
	UpdateRotate();

	// ワールド行列生成
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_worldPos);

	Math::Matrix rotation = Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_worldRot.y),
		DirectX::XMConvertToRadians(m_worldRot.x),
		DirectX::XMConvertToRadians(m_worldRot.z)
	);

	m_mWorld = rotation * trans;

	KdModelWork::Node* pNode = m_modelWork.FindWorkNode("Propeller");


	if (pNode)
	{
		pNode->m_localTransform *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(15.0f));
	}
	m_modelWork.CalcNodeMatrices();

	if (pNode)
	{
		Math::Matrix mCenter;
		Math::Matrix mOuter;

		mCenter = pNode->m_worldTransform * m_mWorld;// Math::Matrix::CreateTranslation(pNode->m_worldTransform.Translation())* m_mWorld;
		mOuter = m_proNodes[0]->m_worldTransform * m_mWorld;// Math::Matrix::CreateTranslation(0, 0, 0.7f)* mCenter;

//		Math::Vector3 vBetween = Math::Vector3::TransformNormal(Math::Vector3::Backward * 0.7, pNode->m_worldTransform);


		m_trail.AddPoint(mCenter);
		m_trail.AddPoint(mOuter);


		if (m_trail.GetNumPoints() > 10)
		{
			m_trail.DelPoint_Back();
			m_trail.DelPoint_Back();
		}

	}
}

void Enemy::DrawEffect()
{
	SHADER->m_effectShader.DrawTrailPolygon(m_trail);
}

void Enemy::Release()
{
	// 取得したm_pTargetの中身をを解放
	SetTarget(nullptr);
}

void Enemy::UpdateMove()
{
	// 見ている先が解放されているか
	if (m_wpTarget.expired()) { return; }

	std::shared_ptr<const GameObject> spTarget = m_wpTarget.lock();

	// プレイヤーに向かう方向ベクトル
	Math::Vector3 targetDir = spTarget->GetPos() - m_worldPos;

	// ターゲットとの距離
	float targetDistSqr = targetDir.LengthSquared();

	if (targetDistSqr < m_stopDist * m_stopDist) { return; }

	// エネミーの向いてる方向ベクトル
	Math::Vector3 moveVec = m_mWorld.Backward();
	moveVec.Normalize();
	
	const float moveSpd = 0.05f;
	moveVec *= moveSpd;

	m_worldPos += moveVec;
}

void Enemy::UpdateRotate()
{
	// 見ている先が解放されているか
	if (m_wpTarget.expired()) { return; }

	std::shared_ptr<const GameObject> spTarget = m_wpTarget.lock();

	// 向いてる方向ベクトル
	Math::Vector3 nowDir = m_mWorld.Backward();
	
	// 移動方向のベクトル(プレイヤーの座標)
	Math::Vector3 targetDir = spTarget->GetPos() - m_worldPos;
	
	// 長さを1に
	nowDir.Normalize();
	targetDir.Normalize();

	// それぞれの現在の角度計算
	float nowAngle = atan2(nowDir.x, nowDir.z);
	nowAngle = DirectX::XMConvertToDegrees(nowAngle);
	float targetAngle = atan2(targetDir.x, targetDir.z);
	targetAngle = DirectX::XMConvertToDegrees(targetAngle);

	// 二点の間の角度を求める(目的地 - 出発地)
	float betweenAng = targetAngle - nowAngle;

	// 回転の補正
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	// 一フレーム当たりの回転量の制御
	float rotSpd = 2.0f;	// ここの角度によって振り向き速度が変わる
	float rotateAng = std::clamp(betweenAng, -rotSpd, rotSpd);

	// 角度更新
	m_worldRot.y += rotateAng;
}
