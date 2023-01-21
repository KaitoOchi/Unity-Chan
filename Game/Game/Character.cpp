#include "stdafx.h"
#include "Character.h"


Character::Character()
{

}

Character::~Character()
{

}

bool Character::Start()
{
	//アニメーションクリップ
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Unitychan/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/Unitychan/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);

	//プレイヤーモデルの設定
	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);

	return true;
}

void Character::Update()
{
	if (m_canMove) {
		Move();
	}

	PlayAnimation();

	m_modelRender.Update();
}

void Character::Move()
{
	Vector3 moveSpeed;

	if (g_pad[0]->IsPress(enButtonLeft)) {
		moveSpeed.x += -5.0f;
		m_direction = -1;
	}
	else if (g_pad[0]->IsPress(enButtonRight)) {
		moveSpeed.x += 5.0f;
		m_direction = 1;
	}

	m_position.x += moveSpeed.x;

	m_modelRender.SetPosition(m_position);
	m_rotation.SetRotationDegY(90.0f * m_direction);
	m_modelRender.SetRotation(m_rotation);

	if (fabsf(moveSpeed.x) > 0.0f) {
		m_animationState = enAnimationClip_Run;
	}
	else {
		m_animationState = enAnimationClip_Idle;
	}
}

void Character::PlayAnimation()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		m_animationState = enAnimationClip_Idle;
	}

	switch (m_animationState) {

	case enAnimationClip_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case enAnimationClip_Run:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
}

void Character::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}