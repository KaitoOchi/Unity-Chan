#include "stdafx.h"
#include "Object.h"

#include "Player.h"

Object::Object()
{
	player = FindGO<Player>("player");

	m_modelRender.SetRaytracingWorld(false);
}

Object::~Object()
{

}

bool Object::Start()
{
	return true;
}

void Object::Update()
{
}

bool Object::GetEnabled(const Vector3& pos)
{
	//�v���C���[����G�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = player->GetPosition() - m_position;
	//�x�N�g���̒������傫��������
	if (diff.Length() > 2500.0f) {
		return false;
	}
	else if (diff.Length() < 2500.0f) {
		return true;
	}
}