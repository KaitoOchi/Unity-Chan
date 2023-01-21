#include "stdafx.h"
#include "Weapon.h"

#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"
#include "graphics/effect/EffectEmitter.h"


Weapon::Weapon()
{

}

Weapon::~Weapon()
{
	DeleteGO(m_collisionObject);
}

bool Weapon::Start()
{
	player = FindGO<Player>("player");

	//�R���W�����̐ݒ�
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetIsEnable(false);


	switch (m_enUser) {
	case enUser_Player:
		m_collisionObject->SetName("weapon_player");
		break;

	case enUser_Enemy:
		//�G�l�~�[�N���X�̎擾
		enemy = FindGO<Enemy>("enemy");
		m_collisionObject->SetName("weapon_enemy");
		break;

	case enUser_Boss:
		//�{�X�N���X�̎擾
		boss = FindGO<Boss>("boss");
		m_collisionObject->SetName("weapon_enemy");
		break;
	}


	//�G�t�F�N�g�̐ݒ�
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/weapon01.efk");
	//�ˌ����̉�
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/weapon01.wav");

	//�G�t�F�N�g�̐ݒ�
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/weapon02.efk");
	//�ˌ����̉�
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/weapon02.wav");

	//�G�t�F�N�g�̐ݒ�
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/weapon03.efk");
	//�ˌ����̉�
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/weapon03.wav");

	Deactivate();

	return true;
}

void Weapon::Update()
{
	//���ˎ��Ɉ�񂾂�
	if (!m_shot) {

		//�����ɂ���ĕς���
		switch (m_weaponNum) {
		case 1:
			//�R���W�����̐ݒ�
			m_collisionObject->CreateBox(m_position, Quaternion(0.0f, 0.0f, 20.0f, 45.0f), Vector3(150.0f, 200.0f, 10.0f));
			break;

		case 2:
			//�R���W�����̐ݒ�
			m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(200.0f, 200.0f, 10.0f));
			break;

		case 3:
			//�R���W�����̐ݒ�
			m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 150.0f, 10.0f));
			break;

		case 4:
			break;
		}

		//�g�p�҂ɂ���ĕς���
		switch (m_enUser) {

		case enUser_Player:
			//�o�Ă���Ԃ͕����ύX�ł��Ȃ��悤�ɂ���
			player->SetCanSelectWeapon(false);
			break;

		case enUser_Enemy:
			break;

		case enUser_Boss:
			break;
		}

		m_oldPos = m_position;

		//�v���C���[�̕������甭�˂̌�����ς���
		if (m_playerDirection == 1) {
			m_effectRotation = 0;
		}
		else {
			m_effectRotation = -1;
		}

		//�G�t�F�N�g�̍Đ�
		m_barrettEffect = NewGO<EffectEmitter>(0);
		m_barrettEffect->Init(m_weaponNum);
		m_barrettEffect->Play();

		//�ˌ������o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(m_weaponNum);
		se->SetVolume(m_volume);
		se->Play(false);

		//�����蔻���L����
		m_collisionObject->SetIsEnable(true);

		m_shot = true;
	}

	//�Đ�������������
	if (!m_barrettEffect->IsPlay()) {
		Reset();
	}

	switch (m_enUser) {
	case enUser_Player:
		//�v���C���[�̈ʒu���擾
		m_userPos = player->GetPosition();
		break;

	case enUser_Enemy:
		break;

	case enUser_Boss:
		m_userPos = boss->GetPosition();
		break;
	}


	switch (m_weaponNum) {
	case 1:
		m_barrettEffect->SetRotation(Quaternion(0.0f, m_effectRotation, 0.0f, 0.0f));
		m_collisionObject->SetPosition(Vector3(m_userPos.x + 60.0f * m_playerDirection, m_userPos.y + 80.0f, m_userPos.z));
		break;

	case 2:
		m_collisionObject->SetPosition(Vector3(m_userPos.x, m_userPos.y + 60.0f, m_userPos.z));
		break;

	case 3:
		if (m_position.x < m_oldPos.x + 700.0f && m_position.x > m_oldPos.x - 700.0f) {
			//�ړ�������
			m_position.x += 25.0f * m_playerDirection;
		}
		m_barrettEffect->SetRotation(Quaternion(0.0f, m_effectRotation, 0.0f, 0.0f));
		m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 80.0f, m_position.z));

		m_userPos = m_oldPos;
		break;

	case 4:
		break;
	}

	m_barrettEffect->SetPosition(m_userPos);

	Collision();
}

void Weapon::Collision()
{
	switch (m_enUser) {
	case enUser_Player:

		break;

	case enUser_Enemy:
		//�R���W�����ƃL�����R�����Փ˂�����
		if (m_collisionObject->IsHit(player->GetCharacterController())) {

			player->ReceiveDamage(m_power);
		}
		break;

	case enUser_Boss:
		//�R���W�����ƃL�����R�����Փ˂�����
		if (m_collisionObject->IsHit(player->GetCharacterController())) {

			player->ReceiveDamage(m_power);
		}
		break;
	}
}

void Weapon::Reset()
{
	m_shot = false;
	m_collisionObject->SetIsEnable(false);

	if (m_enUser == enUser_Player) {
		player->SetCanSelectWeapon(true);
	}

	Deactivate();
}