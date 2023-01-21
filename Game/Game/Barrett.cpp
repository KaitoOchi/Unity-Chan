#include "stdafx.h"
#include "Barrett.h"

#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Box.h"

#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "graphics/effect/EffectEmitter.h"


Barrett::Barrett()
{

}

Barrett::~Barrett()
{
	DeleteGO(m_collisionObject);
}

bool Barrett::Start()
{
	stage = FindGO<Stage>("stage");
	box = FindGO<Box>("box");
	player = FindGO<Player>("player");

	//�R���W�����̐ݒ�
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetIsEnable(false);

	//�G�t�F�N�g�̐ݒ�
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/barrett.efk");

	switch (m_enUser) {
	case enUser_Player:
		//�R���W�����̖��O��ύX
		m_collisionObject->SetName("barrett_player");
		break;

	case enUser_Enemy:
		//�R���W�����̖��O��ύX
		m_collisionObject->SetName("barrett_enemy");
		break;

	case enUser_Boss:
		//�R���W�����̖��O��ύX
		m_collisionObject->SetName("barrett_enemy");
		break;
	}

	//�ˌ����̉�
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");

	Deactivate();

	return true;
}

void Barrett::Update()
{
	if (!IsActive()) {
		return;
	}

	Vector3 moveSpeed;

	//���ˎ��Ɉ�񂾂�
	if (!m_shot) {

		if (m_enUser == enUser_Player) {
			//�o�Ă���Ԃ͕����ύX�ł��Ȃ��悤�ɂ���
			player->SetCanSelectWeapon(false);
		}

		if (m_canChase) {
			//�v���C���[����G�Ɍ������x�N�g�����v�Z
			m_diff = player->GetPosition() - m_position;
			//���K��������
			m_diff.Normalize();
		}

		m_position.y += 30.0f;

		//�G�t�F�N�g�̍Đ�
		m_barrettEffect = NewGO<EffectEmitter>(0);
		m_barrettEffect->Init(0);
		m_barrettEffect->Play();

		//�ˌ������o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(0);
		se->Play(false);
		se->SetVolume(m_volume);

		//�����蔻��̗L����
		m_collisionObject->SetIsEnable(true);

		m_shot = true;
	}

	//�Đ�������������
	if (!m_barrettEffect->IsPlay()) {
		//���Z�b�g����
		Reset();
		return;
	}

	//�ړ�������
	if (m_isRepel) {
		moveSpeed.x = 10.0f * -m_playerDirection;
		moveSpeed.y = 10.0f;
	}
	else if (m_canChase) {
		moveSpeed = m_diff * m_speed;
	}
	else {
		moveSpeed.x = m_speed * m_playerDirection;
	}

	m_position += moveSpeed;
	m_barrettEffect->SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);

	Collision();
}

void Barrett::Collision()
{
	switch (m_enUser) {
	case enUser_Player:
	{
		//�e�̃R���W�������擾����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_enemy");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃR���W�������Փ˂�����
			if (collision->IsHit(m_collisionObject)) {
				m_isRepel = true;
				return;
			}
		}
	}
		break;

	case enUser_Enemy:
		//�R���W�����ƃL�����R�����Փ˂�����
		if (m_collisionObject->IsHit(player->GetCharacterController())) {
			player->ReceiveDamage(m_power);
		}

		{
			//�e�̃R���W�������擾����
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
			//�R���W�����̔z���for���ŉ�
			for (auto collision : collisions)
			{
				//�R���W�����ƃR���W�������Փ˂�����
				if (collision->IsHit(m_collisionObject)) {
					m_isRepel = true;
					return;
				}
			}
		}
		break;

	case enUser_Boss:
		//�R���W�����ƃR���W�������Փ˂�����
		if (m_collisionObject->IsHit(player->GetCharacterController())) {
			player->ReceiveDamage(m_power);
		}

		{
			//�e�̃R���W�������擾����
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
			//�R���W�����̔z���for���ŉ�
			for (auto collision : collisions)
			{
				//�R���W�����ƃR���W�������Փ˂�����
				if (collision->IsHit(m_collisionObject)) {
					m_isRepel = true;
					return;
				}
			}
		}
		break;
	}

	{
		//�{�b�N�X�̃R���W�������擾����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("box");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃR���W�������Փ˂�����
			if (collision->IsHit(m_collisionObject)) {
				Reset();
				return;
			}
		}
	}

	/*�\��
	//�R���W�����ƃX�e�[�W���Փ˂�����
	if (m_collisionObject->IsHit(stage->GetPhysicsStaticObject()))
	{
		//���Z�b�g����
		Reset();
		return;
	}
	*/
}

void Barrett::Reset()
{
	m_shot = false;
	m_isRepel = false;
	m_collisionObject->SetIsEnable(false);

	if (m_enUser == enUser_Player) {
		player->SetCanSelectWeapon(true);
	}

	Deactivate();
}