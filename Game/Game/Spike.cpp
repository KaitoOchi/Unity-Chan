#include "stdafx.h"
#include "Spike.h"

#include "Player.h"
#include "Stage.h"


Spike::Spike()
{

}

Spike::~Spike()
{
	DeleteGO(m_collisionObject);
}

bool Spike::Start()
{
	stage = FindGO<Stage>("stage");

	m_firstPos = m_position;

	//コリジョンの設定
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("spike");

	m_modelRender.SetRaytracingWorld(false);
	switch (m_type) {
	//何もなし
	case 0:
		m_modelRender.Init("Assets/modelData/object/spike.tkm");
		m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(220.0f, 50.0f, 10.0f));
		m_damage = 99;
		break;

	//落下する
	case 1:
		m_modelRender.Init("Assets/modelData/object/fallSpike.tkm");
		m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(250.0f, 50.0f, 10.0f));
		m_damage = 4;
		break;

	//回転する
	case 2:
		m_modelRender.Init("Assets/modelData/object/rotateSpike.tkm");
		m_modelRender.SetScale(Vector3(7.0f, 7.0f, 7.0f));
		m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 100.0f, 10.0f));
		m_damage = 5;
		break;
	}
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

	return true;
}

void Spike::Update()
{
	m_enabled = GetEnabled(m_position);

	switch (m_type) {
	case 0:
		Idle();
		break;

	case 1:
		Fall();
		break;

	case 2:
		Rotate();
		break;
	}

	//当たり判定
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		player->ReceiveDamage(m_damage);
	}
}

void Spike::Idle()
{

}

void Spike::Fall()
{
	//プレイヤーから敵に向かうベクトルを計算。
	Vector3 diff = player->GetPosition() - m_position;

	switch (m_enState) {

	case enState_Idle:
		//既に落下していたなら
		if (m_speed > 2.5f) {
			m_speed += 0.1f;
			if (m_speed > 15.0f) {
				m_enState = enState_IsUp;
			}
		}
		else {
			//近づいたら
			if (fabsf(diff.x) < 200.0f) {
				m_enState = enState_IsFall;
			}
		}
		break;

	case enState_IsFall:
		m_position.y -= m_speed;
		m_speed += 0.1f;

		//コリジョンとステージが衝突したら
		if (m_collisionObject->IsHit(stage->GetPhysicsStaticObject())) {
			m_enState = enState_Idle;
		}

		//消す予定
		if (m_speed > 10.0f) {
			m_enState = enState_Idle;
		}

		m_collisionObject->SetPosition(m_position);
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
		break;

	case enState_IsUp:
		m_position.y += 2.5f;

		//最初の位置まで来たら
		if (m_position.y > m_firstPos.y) {
			m_enState = enState_Idle;
			m_position = m_firstPos;
			m_speed = 2.5f;
		}

		m_collisionObject->SetPosition(m_position);
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
		break;
	}
}

void Spike::Rotate()
{
	//コリジョンとモデルの位置を合わせる
	m_collisionPos = m_position;
	Vector3 rotate = Vector3::AxisY;
	rotation.Apply(rotate);
	m_collisionPos += rotate * 360.0f;

	m_speed += 1.5f;

	m_collisionObject->SetPosition(m_collisionPos);
	rotation.SetRotationDegZ(m_speed);
	m_modelRender.SetRotation(rotation);
	m_modelRender.Update();
}

void Spike::Render(RenderContext& rc)
{
	if(m_enabled)
		m_modelRender.Draw(rc);
}