#pragma once
#include "Object.h"


class InvicibleWall : public IGameObject, public Object
{
public:
	InvicibleWall();
	~InvicibleWall();
	bool Start() override;
	void Update() override;

	//âÒì]Çê›íË
	inline void SetRotation(const Quaternion& rot) { m_rotation = rot; }

private:
	PhysicsStaticObject m_physicsStaticObject;

	Quaternion m_rotation;
};

