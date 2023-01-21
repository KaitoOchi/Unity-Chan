#include "stdafx.h"
#include "CheckPoint.h"

#include "Player.h"
#include "GameManager.h"


CheckPoint::CheckPoint()
{

}

CheckPoint::~CheckPoint()
{
	DeleteGO(m_collisionObject);
}

bool CheckPoint::Start()
{
	//ƒRƒŠƒWƒ‡ƒ“‚Ìİ’è
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(300.0f, 300.0f, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("checkPoint");

	return true;
}

void CheckPoint::Update()
{
	//“–‚½‚è”»’è
	if (m_collisionObject->IsHit(player->GetCharacterController())) {

		GameManager* gameManager = FindGO<GameManager>("gameManager");
		gameManager->SetContinuePosition(m_position);
		gameManager->SetCourceNum(m_type);
		DeleteGO(this);
	}
}