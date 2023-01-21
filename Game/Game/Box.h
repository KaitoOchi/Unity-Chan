#pragma once
#include "Object.h"


class Stage;

class Box : public IGameObject, public Object
{
public:
	Box();
	~Box();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

	//�����蔻����擾
	inline PhysicsStaticObject& GetPhysicsStaticObject() { return m_physicsStaticObject; }

private:
	PhysicsStaticObject		m_physicsStaticObject;

	int						m_direction = 0;		//�ړ�����
	float					m_speed = 5.0f;			//���x
};

