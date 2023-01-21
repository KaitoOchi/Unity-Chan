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

	//�ړ�������ݒ�
	inline void SetDistance(const float& dis) { m_limited = dis; }
	//�ړ����x��ݒ�
	inline void SetSpeed(const Vector3& speed) { m_speed = speed; }
	//vanishFloor�̔ԍ���ݒ�
	inline void SetNumber(const int num) { m_number = num; }
	//�C���X�^���X�ԍ���ݒ�
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
	int						m_shake = 10;		//�U�������
	int						m_direction = 1;	//����
	int						m_number = 0;		//�o�����鏇��
	float					m_time = 0.0f;		//����
	float					m_limited = 0.0f;	//��������
};

