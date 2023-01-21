#include "stdafx.h"
#include "FloorRender.h"

bool FloorRender::Start()
{
	m_modelRender.SetRaytracingWorld(false);
	m_modelRender.Init(
		m_floorName[m_type],
		nullptr,
		0,
		enModelUpAxisZ,
		false,
		m_maxFloor
	);

	return true;
}

void FloorRender::Update()
{
}

void FloorRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}