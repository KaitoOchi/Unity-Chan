#include "stdafx.h"
#include "Ladder.h"

#include "Player.h"


Ladder::Ladder()
{

}

Ladder::~Ladder()
{
	DeleteGO(m_collisionObject);
}

bool Ladder::Start()
{
	//ƒ‚ƒfƒ‹‚Ìİ’è
	m_modelRender.Init("Assets/modelData/object/ladder.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//ƒRƒŠƒWƒ‡ƒ“‚Ìİ’è
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(Vector3(m_position.x, m_position.y + 150.0f, m_position.z), Quaternion::Identity, Vector3(1.0f, m_height, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("ladder");

	return true;
}

void Ladder::Update()
{
	m_enabled = GetEnabled(m_position);

	if (!m_enabled)
		return;

	//“–‚½‚è”»’è
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		player->SetCanClimb(true);
	}
	else {
		player->SetCanClimb(false);
	}
}

void Ladder::Render(RenderContext& rc)
{
	if(m_enabled)
		m_modelRender.Draw(rc);
}
