#pragma once
#include "Object.h"


class Stage;

class Spike : public IGameObject, public Object
{
public:
	enum EnState {
		enState_Idle,
		enState_IsFall,
		enState_IsUp
	};

public:
	Spike();
	~Spike();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

private:
	void Idle();
	void Fall();
	void Rotate();

	Stage*				stage = nullptr;
	Vector3				m_collisionPos;
	Quaternion			rotation;
	EnState				m_enState = enState_Idle;

	int					m_damage = 0;		//ダメージ
	float				m_speed = 0.0f;		//速度
};

