#include "stdafx.h"
#include "InvicibleWall.h"


InvicibleWall::InvicibleWall()
{

}

InvicibleWall::~InvicibleWall()
{

}

bool InvicibleWall::Start()
{
	m_modelRender.SetRaytracingWorld(false);
	m_modelRender.Init("Assets/modelData/object/invicibleWall.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	return true;
}

void InvicibleWall::Update()
{

}