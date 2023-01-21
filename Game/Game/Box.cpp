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
	//���f���̐ݒ�
	m_modelRender.SetRaytracingWorld(false);
	switch (m_type) {

	//���Ȃ�
	case 0:
		//m_modelRender.Init("Assets/modelData/object/box.tkm");
		break;

	//�ړ��\
	case 1:
		m_modelRender.Init("Assets/modelData/object/box_wood.tkm");
		break;

	//���j
	case 2:
		m_modelRender.Init("Assets/modelData/object/box_metal.tkm");
		break;
	}

	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//�R���W�����̐ݒ�
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
		//�����蔻��
		if (m_collisionObject->IsHit(player->GetCharacterController())) {

			if (player->GetPosition().x > m_position.x) {
				m_direction = -1;
			}
			else {
				m_direction = 1;
			}

			m_position.x += m_speed * m_direction;
		}

		/*�\��
		if (!m_collisionObject->IsHit(stage->GetPhysicsStaticObject())) {

			//m_position.y += m_speed;
		}
		*/
		break;
		

	case 2:
		//���e�̃R���W�������擾����
		const auto & collisions = g_collisionObjectManager->FindCollisionObjects("bomb");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃR���W�������Փ˂�����
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