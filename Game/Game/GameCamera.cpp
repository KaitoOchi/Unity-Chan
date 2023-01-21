#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"
#include "Stage.h"

namespace
{
	const float CAMERA_POS_Y = 150.0f;		//初期カメラ位置Y
	const float CAMERA_POS_Z = -900.0f;		//初期カメラ位置Z
	const float CAMERANEAR = 500.0f;		//近平面
	const float CAMERAFAR = 50000.0f;		//遠平面
}

GameCamera::GameCamera()
{

}


GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(CAMERANEAR);
	g_camera3D->SetFar(CAMERAFAR);

	return true;
}

void GameCamera::Update()
{
	Vector3 target = m_player->GetPosition();

	switch (state) {
	case enState_moveX:
		target.y = m_fixedPos.y + 700.0f;
		SetCamera(target);
		break;

	case enState_moveY:
		target.x = m_fixedPos.x;
		target.y += 200.0f;
		SetCamera(target);
		break;

	case enState_Idle:
		target.x = m_fixedPos.x;
		target.y = m_fixedPos.y + 700.0f;
		SetCamera(target);
		break;

	case enState_Scroll:
		g_camera3D->Move(Vector3(13.0f * m_direction.x, 7.0f * m_direction.y, 0.0f));
		break;
	}
}

void GameCamera::SetCamera(const Vector3& target)
{
	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新。
	g_camera3D->Update();
}