#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_camera.SetProjectionMatrix(60.0f);

	m_mLocalCamera=m_mLocalCamera.CreateTranslation({ 0.0f,1.0f,0.0f });
}

void Player::Update()
{
	float moveSpd = 0.03f;
	DirectX::SimpleMath::Vector3 moveVec;
	if (GetAsyncKeyState('W'))
	{
		moveVec.z += moveSpd;
	}
	if (GetAsyncKeyState('S'))
	{
		moveVec.z -= moveSpd;
	}
	if (GetAsyncKeyState('A'))
	{
		moveVec.x -= moveSpd;
	}
	if (GetAsyncKeyState('D'))
	{
		moveVec.x += moveSpd;
	}

	//カメラの回転を加味する
	moveVec = moveVec.TransformNormal(moveVec, m_RotateCamera);

	DirectX::SimpleMath::Matrix trans;

	trans = trans.CreateTranslation(moveVec);

	float horizontal = 0.0f;

	//DirectX::SimpleMath::Matrix rotate;
	//rotate = rotate.CreateRotationY(horizontal);


	POINT nowPos;
	GetCursorPos(&nowPos);

	static POINT prevPos=nowPos;

	POINT mouseMove;
	mouseMove.x = nowPos.x - prevPos.x;
	mouseMove.y = nowPos.y - prevPos.y;

	prevPos = nowPos;


	Math::Matrix rotateY;
	rotateY = rotateY.CreateRotationY(DirectX::XMConvertToRadians(mouseMove.x * 0.25f));
	m_RotateCamera *= rotateY;

	Math::Matrix rotateX;
	rotateX = rotateX.CreateRotationX(DirectX::XMConvertToRadians(mouseMove.y * 0.25f));
	m_RotateCamera *= rotateX;

	m_mWorld = m_mWorld*trans;

	m_camera.SetCameraMatrix(m_RotateCamera*m_mLocalCamera * m_mWorld);
}

void Player::Draw()
{
}

void Player::Release()
{
}
