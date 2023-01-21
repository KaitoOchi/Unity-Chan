#include "stdafx.h"
#include "Spring.h"

#include "Player.h"


Spring::Spring()
{

}

Spring::~Spring()
{
	DeleteGO(m_collisionObject);
}

bool Spring::Start()
{
	//ƒ‚ƒfƒ‹‚Ìİ’è
	m_modelRender.SetRaytracingWorld(false);
	m_modelRender.Init("Assets/modelData/object/spring.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//ƒRƒŠƒWƒ‡ƒ“‚Ìİ’è
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(Vector3(m_position.x, m_position.y + 25.0f, m_position.z), Quaternion::Identity, Vector3(100.0f, 5.0f, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("spring");

	return true;
}

void Spring::Update()
{
	m_enabled = GetEnabled(m_position);

	//“–‚½‚è”»’è
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		player->SetIsSpring(true);
	}
}

void Spring::Render(RenderContext& rc)
{
	if(m_enabled)
		m_modelRender.Draw(rc);
}
