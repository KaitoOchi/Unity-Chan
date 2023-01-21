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
	//プレイヤーから敵に向かうベクトルを計算。
	Vector3 diff = player->GetPosition() - m_position;
	//ベクトルの長さが大きかったら
	if (diff.Length() > 2500.0f) {
		return false;
	}
	else if (diff.Length() < 2500.0f) {
		return true;
	}
}