#include "stdafx.h"
#include "Outside.h"

#include "Player.h"
#include "GameCamera.h"

Outside::Outside()
{

}

Outside::~Outside()
{

}

bool Outside::Start()
{
	player = FindGO<Player>("player");

	Vector3 fixedPos = m_position;
	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
	gameCamera->SetFixedPosition(fixedPos);
	gameCamera->EnStateIdle();

	return true;
}

void Outside::Update()
{
	if (player->GetPosition().y < m_position.y) {
		player->ReceiveDamage(99);
	}
}