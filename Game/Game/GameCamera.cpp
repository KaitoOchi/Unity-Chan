#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"
#include "Stage.h"

namespace
{
	const float CAMERA_POS_Y = 150.0f;		//�����J�����ʒuY
	const float CAMERA_POS_Z = -900.0f;		//�����J�����ʒuZ
	const float CAMERANEAR = 500.0f;		//�ߕ���
	const float CAMERAFAR = 50000.0f;		//������
}

GameCamera::GameCamera()
{

}


GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(CAMERANEAR);
	g_camera3D->SetFar(CAMERAFAR);

	return true;
}

void GameCamera::Update()
{
	Vector3 target = m_player->GetPosition();

	switch (state) {
	case enState_moveX:
		target.y = m_fixedPos.y + 700.0f;
		SetCamera(target);
		break;

	case enState_moveY:
		target.x = m_fixedPos.x;
		target.y += 200.0f;
		SetCamera(target);
		break;

	case enState_Idle:
		target.x = m_fixedPos.x;
		target.y = m_fixedPos.y + 700.0f;
		SetCamera(target);
		break;

	case enState_Scroll:
		g_camera3D->Move(Vector3(13.0f * m_direction.x, 7.0f * m_direction.y, 0.0f));
		break;
	}
}

void GameCamera::SetCamera(const Vector3& target)
{
	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;

	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}