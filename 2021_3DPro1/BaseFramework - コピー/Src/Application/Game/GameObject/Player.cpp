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

	m_spCamera->SetProjectionMatrix(60.0f, 3000.0f);	// 視野角の設定（左右に60度＝120度）,最大描画距離(短いほど判定が正確になる)

	// カメラの注視点から5m離れる
	m_spCamera->SetLocalPos(Math::Vector3(0.0f, 0.0f, -5.0f));

	// キャラクターから注視点へのローカル座標を上に3m上げる
	m_spCamera->SetLocalGazePos(Math::Vector3(0.0f, 3.0f, 0.0f));

	// カメラの制限角度
	m_spCamera->SetClampAngle(-75.0f,90.0f);

	// カメラの移動スピード
	m_spCamera->SetRotationSpeed(0.25);

	m_radius = 0.5f;

	m_animator.SetAnimation(m_modelWork.GetData()->GetAnimation("Walk"));

	m_spActionState = std::make_shared<ActionWait>();

	//デバッグObjectのインスタンス化
	m_spDebugObject = std::make_shared<DebugObject>();
		
}

// 更新処理
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

	// 当たり判定更新
	UpdateCollition();

	// 行列の更新(最終的な座標を確定してから)
	UpdateMatrix();

	ShotArrow();

	// カメラを構成する行列の合成結果をセット
	if (m_spCamera)
	{
		m_spCamera->Update();

		Math::Matrix trans = Math::Matrix::CreateTranslation(m_worldPos);

		// プレイヤーの絶対行列のセット
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

// 移動の更新処理
void Player::UpdateMove(Math::Vector3& dstMove)
{
	// キャラの移動処理
	float moveSpd = 0.05f;

	Math::Vector3 moveVec;

	if (GetAsyncKeyState('W') & 0x8000) { moveVec.z += 1.0f; }	// 前移動
	if (GetAsyncKeyState('S') & 0x8000) { moveVec.z -= 1.0f; }	// 後ろ移動
	if (GetAsyncKeyState('A') & 0x8000) { moveVec.x -= 1.0f; }	// 左移動
	if (GetAsyncKeyState('D') & 0x8000) { moveVec.x += 1.0f; }	// 右移動

	// 斜め移動による加速を補正
	moveVec.Normalize();
	moveVec *= moveSpd;

	if (m_spCamera)
	{
		// 進行方向をカメラ回転を加味して修正する
		moveVec = moveVec.TransformNormal(moveVec, m_spCamera->GetRotationYMatrix());
	}

	// 座標確定
	m_worldPos.x += moveVec.x;
	m_worldPos.z += moveVec.z;

	dstMove = moveVec;
}

// 回転の更新処理
void Player::UpdateRotate(const Math::Vector3& srcMove)
{
	if (!m_spCamera) { return;}

	// ベクトルの長さがゼロだったら
	if (srcMove.LengthSquared() == 0.0f) { return; }

	// キャラの正面方向ベクトル：出発地
	Math::Vector3 nowDir = m_mWorld.Backward();
	
	// 移動方向のベクトル：目標地
	Math::Vector3 targetDir = srcMove;

	nowDir.Normalize();
	targetDir.Normalize();

	// それぞれのDegree角度を求める
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// ２つの間の角度を求める
	float rotateAng = targetAng - nowAng;

	// 回転の補正
	if (rotateAng > 180) 
	{
		rotateAng -= 360; 
	}
	else if (rotateAng < -180)
	{
		rotateAng += 360;
	}

	// 回転量代入
	rotateAng = std::clamp(rotateAng, -8.0f, 8.0f);
	
	m_worldRot.y += rotateAng;
}

// 行列更新
void Player::UpdateMatrix()
{
	// ワールド行列を作成
	Math::Matrix trans;
	trans = Math::Matrix::CreateTranslation(m_worldPos);
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = rotation * trans;
}

void Player::UpdateDebug()
{
	if (!m_spDebugObject) { return; }
	
	//出バグラインを登録する
	Math::Vector3 localEndPos = GetPos();
	localEndPos.y += 10.0f;
	m_spDebugObject->AddDebugLine(GetPos(), localEndPos);
	

	/*//デバッグスフィアを描画する
	Math::Vector3 localSpherePos = GetPos() + m_bumpSphereInfo, m_pos;
	m_spDebugObject->AddDebugSphere(localSpherePos,m_bumpSphereInfo)
	*/
}

// 当たり判定の更新
void Player::UpdateCollition()
{
	for (const std::shared_ptr<GameObject>& spObj : GameSystem::GetInstance().GetObjects())
	{
		if (spObj->GetClassID() != GameObject::eEnemy) { continue; }

		SphereInfo info(GetPos(),m_radius);

		BumpResult result;

		// 相手の判定関数を利用する
		if (spObj->CheckCollisionBump(info, result))
		{
			m_worldPos += result.m_pushVec;
		}
	}

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eStage) { continue; }

		Math::Vector3 rayPos = m_prevPos;
		//　歩いて移動できる地面の限界の段差
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

		// 矢をインスタンス化
		std::shared_ptr<Arrow> pArrow = std::make_shared<Arrow>();

		pArrow->Init();

		const std::list<std::shared_ptr<GameObject>>& objects =
			GameSystem::GetInstance().GetObjects();

		// 参照を付けることでshared_ptrを複製しない
		for (const std::shared_ptr<GameObject>& spObject : objects)
		{
			if (spObject->GetClassID() == GameObject::eEnemy)
			{
				// Enemyのものがあったらセット
				pArrow->SetTarget(spObject);
				
				break;
			}
		}

		// プレイヤーの頭から矢を発射
		const KdModelWork::Node* pNode = m_modelWork.FindNode("Head");

		// ノードがあれば頭から
		if (pNode)
		{	// m_worldTransform … モデルの原点からのローカル行列
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
	//移動キーが押された？
	if (GetAsyncKeyState('W')) { owner.m_spActionState = std::make_shared<ActionMove>(); }

	//ジャンプキー押された？

}


void Player::ActionMove::Update(Player& owner)
{
	//移動キーが押されていない？
	if (!GetAsyncKeyState('W')) { owner.m_spActionState = std::make_shared<ActionWait>(); }

	Math::Vector3 vMove;

	// 移動の更新処理
	owner.UpdateMove(vMove);

	// 回転の更新処理
	owner.UpdateRotate(vMove);

	//vmovenの長さが0だったら移動してないとみなす
	if (vMove.LengthSquared() == 0)
	{
		owner.ChangeWait();
	}
}
