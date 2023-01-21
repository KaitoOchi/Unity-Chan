#include "stdafx.h"
#include "Wall.h"

#include "Player.h"
#include "GameCamera.h"
#include "Game.h"
#include "Outside.h"
#include "InvicibleWall.h"


Wall::Wall()
{

}

Wall::~Wall()
{

}

bool Wall::Start()
{
	player = FindGO<Player>("player");
	gameCamera = FindGO<GameCamera>("gameCamera");

	Outside* outside = FindGO<Outside>("outside");
	m_scrollNum = outside->GetNumber();

	m_outsidePos = outside->GetPosition();
	m_scrollPos = m_position;

	Vector3 colScale;

	switch (m_wallNum) {
	case 0:
		m_scrollPos.x = m_outsidePos.x;
		m_scrollPos.y -= 1400.0f;
		m_enabledPos = 600.0f;
		colScale = Vector3(700.0f, 10.0f, 10.0f);
		break;

	case 1:
		m_scrollPos.x -= 900.0f;
		m_scrollPos.y -= 600.0f;
		m_enabledPos = 1000.0f;
		colScale = Vector3(10.0f, 900.0f, 10.0f);
		break;

	case 2:
		m_scrollPos.x = m_outsidePos.x;
		m_scrollPos.y = m_outsidePos.y;
		m_enabledPos = 600.0f;
		colScale = Vector3(900.0f, 10.0f, 10.0f);
		break;

	case 3:
		m_scrollPos.x += 900.0f;
		m_scrollPos.y -= 600.0f;
		m_enabledPos = 1000.0f;
		colScale = Vector3(10.0f, 900.0f, 10.0f);
		break;
	}

	if (!m_canScroll) {
		//モデルの設定
		InvicibleWall* invicibleWall = NewGO<InvicibleWall>(0, "invicibleWall");
		invicibleWall->Object::SetPosition(m_position);
		invicibleWall->SetRotation(m_rotation);
	}

	//コリジョンの設定
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, colScale);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("wall");


	return true;
}

void Wall::Update()
{
	Vector3 diff = player->GetPosition() - m_position;
	if (diff.Length() > m_enabledPos) {
		return;
	}

	if (m_wallNum == 4) {
		return;
	}
	
	if (gameCamera->IsIdle() && !player->GetScrollState()) {

		if (m_wallNum == 0 && player->GetPosition().y > m_position.y - 500.0f) {
			SetCamera();
		}
		else if (m_wallNum == 1 && player->GetPosition().x > m_position.x - 900.0f) {
			SetCamera();
		}
		else if (m_wallNum == 2 && player->GetPosition().y < m_position.y + 500.0f) {
			SetCamera();
		}
		else if (m_wallNum == 3 && player->GetPosition().x < m_position.x + 900.0f) {
			SetCamera();
		}
		return;
	}

	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		SetScroll();
		return;
	}

	switch (m_wallNum) {
	case 0:
		if (player->GetPosition().y < m_position.y - 500.0f) {
			SetState();
		}
		break;

	case 1:
		if (player->GetPosition().x < m_position.x - 900.0f) {
			SetState();
		}
		break;

	case 2:
		if (player->GetPosition().y > m_position.y + 500.0f) {
			SetState();
		}
		break;

	case 3:
		if (player->GetPosition().x > m_position.x + 900.0f) {
			SetState();
		}
		break;
	}
}

void Wall::SetCamera()
{
	//カメラを待機状態にする
	gameCamera->EnStateIdle();
	gameCamera->SetFixedPosition(m_scrollPos);
}

void Wall::SetScroll()
{
	Vector2 direction;

	if (m_wallNum == 0) {
		direction.x = 0.0f;
		direction.y = 200.0f;
	}
	else if (m_wallNum == 1) {
		direction.x = 200.0f;
		direction.y = 0.0f;
	}
	else if (m_wallNum == 2) {
		direction.x = 0.0f;
		direction.y = -200.0f;
	}
	else if (m_wallNum == 3) {
		direction.x = -200.0f;
		direction.y = 0.0f;
	}

	//スクロールステートにする
	player->SetScrollState();
	player->AddMoveSpeed(Vector3(direction.x, direction.y, 0.0f));
	gameCamera->SetFixedPosition(Vector3(player->GetPosition().x, m_outsidePos.y, 0.0f));
	gameCamera->SetDirection(m_wallNum);
	gameCamera->EnStateScroll();

	DeleteGO(this);
}

void Wall::SetState()
{
	switch (m_scrollNum) {
	case 0:
		gameCamera->EnStateMoveX();
		break;

	case 1:
		gameCamera->EnStateMoveY();
		break;

	case 2:
		gameCamera->EnStateIdle();
		break;
	}
}
