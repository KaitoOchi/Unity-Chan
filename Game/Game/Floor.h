#pragma once
#include "Object.h"


class FloorRender;

class Floor : public IGameObject, public Object
{
public:
	Floor();
	~Floor();
	bool Start() override;
	void Update() override;

	//移動距離を設定
	inline void SetDistance(const float& dis) { m_limited = dis; }
	//移動速度を設定
	inline void SetSpeed(const Vector3& speed) { m_speed = speed; }
	//vanishFloorの番号を設定
	inline void SetNumber(const int num) { m_number = num; }
	//インスタンス番号を設定
	inline void SetInstanceNo(const int num) { m_instanceNo = num; }

private:
	void MoveFloor();
	void FallFloor();
	void VanishFloor();
	void Conveyor();

	ModelRender				m_conveyorModelRender;
	PhysicsStaticObject		m_physicsStaticObject;
	FloorRender*			m_floorRender = nullptr;
	Vector3					m_speed;
	Vector3					m_scale = Vector3::One;
	Quaternion				m_rotation;

	bool					m_isFall = false;
	int						m_frameCount = 0;
	int						m_instanceNo = 0;
	int						m_shake = 10;		//振動する回数
	int						m_direction = 1;	//方向
	int						m_number = 0;		//出現する順番
	float					m_time = 0.0f;		//時間
	float					m_limited = 0.0f;	//制限時間
};

