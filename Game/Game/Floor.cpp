#include "stdafx.h"
#include "Floor.h"

#include "Player.h"
#include "FloorRender.h"


namespace
{
	const Vector3 COLLISION_HEIGHT = Vector3(0.0f, 20.0f, 0.0f);

	const float FALLSPEED = 20.0f;		//�������x
}


Floor::Floor()
{

}

Floor::~Floor()
{
	DeleteGO(m_collisionObject);

	// ��Ɏ���ł��邩������Ȃ��̂ŁA��������null�`�F�b�N������B
	auto floorRender = FindGO<FloorRender>("floorRender");
	if (floorRender) {
		if (m_type != 3) {
			floorRender->RemoveInstance(m_instanceNo);
		}
	}
}

bool Floor::Start()
{
	m_floorRender = FindGO<FloorRender>("floorRender");

	m_firstPos = m_position;

	Vector3 colScale = Vector3(100.0f, 10.0f, 10.0f);

	//��ނ�ݒ�
	m_modelRender.SetRaytracingWorld(false);
	switch (m_type) {
	//��������
	case 0:
		m_modelRender.Init("Assets/modelData/object/moveFloor.tkm");
		m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
		break;

	//�����鑫��
	case 1:
		m_modelRender.Init("Assets/modelData/object/moveFloor.tkm");
		m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
		break;

	//�����鑫��
	case 2:
		m_modelRender.Init("Assets/modelData/object/vanishFloor.tkm");
		m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
		break;

	//�x���g�R���x�A
	case 3:
		m_modelRender.Init("Assets/modelData/object/conveyor.tkm");
		m_conveyorModelRender.SetRaytracingWorld(false);
		m_conveyorModelRender.Init("Assets/modelData/object/conveyor_collision.tkm");
		m_conveyorModelRender.SetPosition(m_position);
		m_physicsStaticObject.CreateFromModel(m_conveyorModelRender.GetModel(), m_conveyorModelRender.GetModel().GetWorldMatrix());
		m_conveyorModelRender.Update();
		colScale = Vector3(650.0f, 10.0f, 10.0f);
		break;
	}
	m_physicsStaticObject.SetPosition(m_position);

	//�R���W�����̐ݒ�
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position + COLLISION_HEIGHT, Quaternion::Identity, colScale);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("floor");

	return true;
}

void Floor::Update()
{
	m_enabled = GetEnabled(m_position);

	switch (m_type) {
	case 0:
		MoveFloor();
		break;

	case 1:
		FallFloor();
		break;

	case 2:
		VanishFloor();
		break;

	case 3:
		Conveyor();
		break;
	}

	m_physicsStaticObject.SetPosition(m_position);

	m_floorRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void Floor::MoveFloor()
{
	Vector3 moveSpeed = Vector3::Zero;

	if (m_speed.x > 0.0f) {
		//���l�𒴂����甽�]������
		if (m_position.x - m_firstPos.x > m_limited) {
			m_direction = -1;
		}
		else if (m_firstPos.x - m_position.x > m_limited) {
			m_direction = 1;
		}
	}
	else if (m_speed.y > 0.0f) {
		//���l�𒴂����甽�]������
		if (m_position.y - m_firstPos.y > m_limited) {
			m_direction = -1;
		}
		else if (m_firstPos.y - m_position.y > m_limited) {
			m_direction = 1;
		}
	}

	moveSpeed = m_speed * m_direction;

	m_position += moveSpeed * g_gameTime->GetFrameDeltaTime();

	m_collisionObject->SetPosition(m_position + COLLISION_HEIGHT);

	//�����蔻��
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		player->AddMoveSpeed(Vector3(moveSpeed.x, 0.0f, 0.0f));
	}
}

void Floor::FallFloor()
{
	if (m_isFall) {

		//�����܂ł̉��o
		if (m_time < 1.5f) {

			if (m_frameCount % 2 == 0) {

				m_shake *= -1;
			}
			m_frameCount++;

			m_position.x += m_shake;
		}
		//������
		else if (m_time > 1.5f && m_time < 5.0f) {

			m_position.y -= FALLSPEED * (m_time - 1.5f);
		}
		//�����ォ�畜��
		else if (m_time > 7.5f) {

			m_time = 0.0f;
			m_frameCount = 0;
			m_isFall = false;
			m_enabled = true;
			m_position = m_firstPos;
		}

		m_time += g_gameTime->GetFrameDeltaTime();
	}
	else {

		//�����蔻��
		if (m_collisionObject->IsHit(player->GetCharacterController())) {

			m_isFall = true;
		}
	}
}

void Floor::VanishFloor()
{
}

void Floor::Conveyor()
{
	//�����蔻��
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		player->AddMoveSpeed(Vector3(100.0f * m_number, 0.0f, 0.0f));
	}
}