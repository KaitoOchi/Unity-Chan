#include "stdafx.h"
#include "Box.h"

#include "Player.h"


Box::Box()
{

}

Box::~Box()
{
	DeleteGO(m_collisionObject);
}

bool Box::Start()
{
	//モデルの設定
	m_modelRender.SetRaytracingWorld(false);
	switch (m_type) {

	//情報なし
	case 0:
		//m_modelRender.Init("Assets/modelData/object/box.tkm");
		break;

	//移動可能
	case 1:
		m_modelRender.Init("Assets/modelData/object/box_wood.tkm");
		break;

	//爆破
	case 2:
		m_modelRender.Init("Assets/modelData/object/box_metal.tkm");
		break;
	}

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//コリジョンの設定
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(150.0f, 50.0f, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("box");

	return true;
}

void Box::Update()
{
	m_enabled = GetEnabled(m_position);

	switch (m_type) {

	case 0:
		break;

	case 1:
		//当たり判定
		if (m_collisionObject->IsHit(player->GetCharacterController())) {

			if (player->GetPosition().x > m_position.x) {
				m_direction = -1;
			}
			else {
				m_direction = 1;
			}

			m_position.x += m_speed * m_direction;
		}

		/*予定
		if (!m_collisionObject->IsHit(stage->GetPhysicsStaticObject())) {

			//m_position.y += m_speed;
		}
		*/
		break;
		

	case 2:
		//爆弾のコリジョンを取得する
		const auto & collisions = g_collisionObjectManager->FindCollisionObjects("bomb");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとコリジョンが衝突したら
			if (collision->IsHit(m_collisionObject))
			{
				DeleteGO(this);
				return;
			}
		}
		break;
	}

	m_modelRender.SetPosition(m_position);
	m_physicsStaticObject.SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);
	m_modelRender.Update();
}

void Box::Render(RenderContext& rc)
{
	if(m_enabled)
		m_modelRender.Draw(rc);
}