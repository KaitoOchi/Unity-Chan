#pragma once
#include "Object.h"


class InvicibleWall : public IGameObject, public Object
{
public:
	InvicibleWall();
	~InvicibleWall();
	bool Start() override;
	void Update() override;

	//��]��ݒ�
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }

private:
	PhysicsStaticObject m_physicsStaticObject;

	Quaternion m_rotation;
};

