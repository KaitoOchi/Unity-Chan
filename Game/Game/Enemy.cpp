#include "stdafx.h"
#include "Enemy.h"

#include "Player.h"
#include "Item.h"
#include "Barrett.h"
#include "Stage.h"
#include "GameManager.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	DeleteGO(m_collisionObject);
}

bool Enemy::Start()
{
	player = FindGO<Player>("player");
	stage = FindGO<Stage>("stage");
	
	m_firstPos = m_position;
	m_modelRender.SetPosition(m_firstPos);
	m_modelRender.Update();

	GameManager* gameManager = FindGO<GameManager>("gameManager");
	m_volume = gameManager->m_sfx;

	m_modelRender.SetRaytracingWorld(false);
	//�G�̎�ނɉ����ĕω�
	switch (m_enemyNum) {
	//StoneMonster
	case 0:
		//�A�j���[�V�����̐ݒ�
		animationClips[enAnimationClip_Idle].Load("Assets/animData/StoneMonsterAnim/StoneMonstorIdle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/StoneMonsterAnim/StoneMonstorRun.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/StoneMonster/StoneMonster.tkm", animationClips, enAnimationClip_Num);
		m_modelRender.SetScale(2.0f, 2.0f, 2.0f);

		//�X�e�[�^�X�̐ݒ�
		SetStatus(3, 2, 3.0f, false, 0, 2, 1.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_XY, enAttack_Not);
		break;

	//MushroomMan
	case 1:
		//�A�j���[�V�����̐ݒ�
		animationClips[enAnimationClip_Idle].Load("Assets/animData/MushroomManAnim/MM_Idle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/MushroomManAnim/MM_Walk.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/MushroomMan/MushroomMan.tkm", animationClips, enAnimationClip_Num);
		m_modelRender.SetScale(0.5f, 0.5f, 0.5f);

		//�X�e�[�^�X�̐ݒ�
		SetStatus(3, 3, 2.5f, false, 0, 1, 0.5f, Vector3(70.0f, 70.0f, 70.0f), enMove_X, enAttack_Not);
		break;

	//Iruka
	case 2:
		//�A�j���[�V�����̐ݒ�
		animationClips[enAnimationClip_Idle].Load("Assets/animData/Iruka/idle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/Iruka/move.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/Iruka/iruka.tkm", animationClips, enAnimationClip_Num);
		m_modelRender.SetScale(0.5f, 0.5f, 0.5f);

		//�X�e�[�^�X�̐ݒ�
		SetStatus(3, 2, 3.0f, false, 0, 3, 3.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_Y, enAttack_Not);
		break;

	//Zako
	case 3:
		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/Enemy03/enemy03.tkm");

		//�X�e�[�^�X�̐ݒ�
		SetStatus(2, 2, 2.0f, true, 0, 1, 3.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_Chase_Air, enAttack_Not);
		break;

	//Taiho
	case 4:
		//�A�j���[�V�����̐ݒ�
		animationClips[enAnimationClip_Idle].Load("Assets/animData/Enemy04/idle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/Enemy04/move.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/Enemy04/enemy04.tkm", animationClips, enAnimationClip_Num);

		//�X�e�[�^�X�̐ݒ�
		SetStatus(4, 3, 0.0f, false, 10, 0, 2.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_Stop, enAttack_Barrett);

		//�e�̐ݒ�
		barrett = NewGO<Barrett>(0, "barrett");
		barrett->SetUser(Barrett::enUser_Enemy);
		barrett->SetPower(m_damage);
		barrett->SetChase(false);
		barrett->SetSpeed(10.0f);
		break;

	//Mimizu
	case 5:
		//�A�j���[�V�����̐ݒ�
		animationClips[enAnimationClip_Idle].Load("Assets/animData/Enemy05/idle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/Enemy05/move.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/Enemy05/enemy05.tkm", animationClips, enAnimationClip_Num);

		//�X�e�[�^�X�̐ݒ�
		SetStatus(1, 1, 3.0f, false, 0, 2, 2.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_Chase_Ground, enAttack_Not);
		break;

	//Spawnner
	case 6:
		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/Enemy06/enemy06.tkm");

		//�X�e�[�^�X�̐ݒ�
		SetStatus(4, 3, 0.0f, false, 0, 1, 2.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_Stop, enAttack_Summon);

		for (int i = 0; i < 3; i++) {
			enemy[i] = NewGO<Enemy>(0, "enemy");
			enemy[i]->SetPosition(m_position);
			enemy[i]->SetNumber(5);
		}
		break;
	//Rabbit
	case 7:
		animationClips[enAnimationClip_Idle].Load("Assets/animData/SlimeRabbit/SlimeRabbit_idle.tka");
		animationClips[enAnimationClip_Idle].SetLoopFlag(true);
		animationClips[enAnimationClip_Run].Load("Assets/animData/SlimeRabbit/SlimeRabbit_move.tka");
		animationClips[enAnimationClip_Run].SetLoopFlag(true);

		//���f���̐ݒ�
		m_modelRender.Init("Assets/modelData/enemy/SlimeRabbit/SlimeRabbit.tkm", animationClips, enAnimationClip_Num);
		m_modelRender.SetScale(Vector3(3.0f, 3.0f, 3.0f));

		//�X�e�[�^�X�̐ݒ�
		SetStatus(2, 3, 2.0f, false, 0, 2, 2.0f, Vector3(70.0f, 70.0f, 70.0f), enMove_XY, enAttack_Not);
		break;
	}


	//�R���W�����̐ݒ�
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, m_colScale);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("enemy");

	m_enabled = false;
	return true;
}

void Enemy::Update()
{
	if (!m_active) {
		//�v���C���[����G�Ɍ������x�N�g�����v�Z�B
		Vector3 diff = player->GetPosition() - m_position;
		//�x�N�g���̒������傫��������
		if (!m_canSpawn && diff.Length() > 1000.0f) {
			m_canSpawn = true;
		}
		else if (m_canSpawn && diff.Length() < 1000.0f) {
			Reset();
		}
		return;
	}

	Move();

	Attack();

	Actor::Blink();

	//��~���ԂłȂ��Ȃ�
	if (m_stopTime <= 0.0f) {

		ManageState();

		PlayAnimation();

		Collision();

	}
	else {
		m_stopTime -= g_gameTime->GetFrameDeltaTime();
	}

	//���������ꂽ��
	if (fabsf(player->GetPosition().x - m_position.x) > 2000.0f || fabsf(player->GetPosition().y - m_position.y) > 1000.0f) {

		Dead();
	}
	m_modelRender.Update();
}

void Enemy::Move()
{
	if (m_stopTime <= 0.0f) {

		m_moveTime += g_gameTime->GetFrameDeltaTime();

		if (m_moveTime >= m_moveT) {
			//�ړ����������ύX����
			if (m_moveDirection == 1) {
				m_moveDirection = -1;
			}
			else {
				m_moveDirection = 1;
			}
			m_moveTime = 0.0f;
		}

		//�v���C���[����G�Ɍ������x�N�g�����v�Z
		Vector3 diff = player->GetPosition() - m_position;

		switch (m_moveType) {
		case enMove_X:
			m_moveSpeed.x = m_speed * m_direction;
			IsOnGround();
			break;

		case enMove_Y:
			m_moveSpeed.y = (-powf(m_moveTime, 2.0f) + m_moveNum * m_moveTime) * m_speed * m_moveDirection;
			break;

		case enMove_XY:
			m_moveSpeed.x = m_speed * m_direction;
			m_moveSpeed.y = (-powf(m_moveTime, 2.0f) + m_moveNum * m_moveTime) * m_speed * m_moveDirection;
			break;

		case enMove_Chase_Ground:
			//���K��������
			diff.Normalize();
			m_moveSpeed.x = diff.x * m_speed;
			IsOnGround();
			Direction();
			break;

		case enMove_Chase_Air:
			//���K��������
			diff.Normalize();
			m_moveSpeed = diff * m_speed;
			Direction();
			break;

		case enMove_Stop:
			m_moveSpeed *= 0.0f;
			break;
		}
	}
	m_moveSpeed.x += m_addSpeed;

	if (m_isOnGround) {
		m_moveSpeed.y += 0.01f;
	}

	m_position.z = 0.0f;
	m_position += m_moveSpeed;

	m_modelRender.SetPosition(m_position);
	m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 30.0f, 0.0f));
}

void Enemy::IsOnGround()
{
	//�ڒn���Ă���Ȃ�
	if (m_isOnGround) {
		m_gravity = 0.0f;
		m_moveSpeed.y = 0.0f;
	}
	else {
		m_gravity += 0.1f;
		m_moveSpeed.y += m_gravity;
	}
}

void Enemy::Attack()
{
	switch (m_attackType) {

	case enAttack_Barrett:
		if (m_moveDirection == -1) {
			//�����𐮐��ɕϊ�����
			int ms = (m_moveTime - (int)m_moveTime) * 10;

			if (ms % m_attackInterval == 0) {

				Direction();

				barrett->SetPosition(m_position);
				barrett->SetDirection(m_direction);
				barrett->Activate();

				//�ˌ������o��
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(0);
				se->Play(false);
				se->SetVolume(m_volume);
			}
		}
		break;

	case enAttack_Weapon:
		break;

	case enAttack_Summon:
		if (m_moveTime > m_moveT - 0.1f) {

			for (int i = 0; i < 3; i++) {

				if (!enemy[i]->IsActive()) {
					enemy[i]->Reset();
					enemy[i]->Activate();
					m_moveTime = 0.0f;
					break;
				}
			}
		}
		break;
	}
}

void Enemy::Collision()
{
	//�v���C���[�ƐڐG������
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		//�_���[�W��^����
		player->ReceiveDamage(m_damage);
	}

	{
		//�e�̃R���W�������擾����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("barrett_player");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃR���W�������Փ˂�����
			if (collision->IsHit(m_collisionObject))
			{
				ReceiveDamage(player->GetWeaponPower());
				return;
			}
		}
	}

	{
		//�e�̃R���W�������擾����
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
		//�R���W�����̔z���for���ŉ�
		for (auto collision : collisions)
		{
			//�R���W�����ƃR���W�������Փ˂�����
			if (collision->IsHit(m_collisionObject))
			{
				ReceiveDamage(player->GetWeaponPower());

				//�E�F�|��3�Ȃ�
				if (player->GetWeaponPower() == 2) {

					m_addSpeed = 5.0f * player->Actor::GetDirection();
				}
				return;
			}
			else {
				m_addSpeed = 0.0f;
			}
		}
	}

	/*�\��
	//�R���W�����ƃX�e�[�W���Փ˂�����
	if (m_collisionObject->IsHit(stage->GetPhysicsStaticObject())) {
		m_isOnGround = true;
	}
	else{
		m_isOnGround = false;
	}
	*/
}

void Enemy::ManageState()
{
	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= 0.001f && fabsf(m_moveSpeed.z) >= 0.001f) {
		//�ړ���Ԃɂ���
		m_enemyState = 1;
	}
	else {
		//�ҋ@��Ԃɂ���
		m_enemyState = 0;
	}
}

//�A�j���[�V����
void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
	case 0:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
	case 1:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	}
}

void Enemy::Reset()
{
	Direction();

	//�����蔻��̗L����
	m_collisionObject->SetIsEnable(true);

	m_hp = m_maxHP;
	m_stopTime = 0.0f;
	m_moveTime = 0.0f;
	m_enabled = true;
	m_active = true;
}

void Enemy::Dead()
{
	if (m_hp <= 0) {
		Item* item = NewGO<Item>(0, "item");
		item->SetPosition(m_position);
	}

	//�����蔻��̖�����
	m_collisionObject->SetIsEnable(false);

	m_position  = m_firstPos;
	m_modelRender.SetPosition(m_firstPos);
	m_canSpawn = false;
	m_enabled = false;
	m_active = false;

	if (m_enemyNum == 5)
		Deactivate();

	if (m_isCanRevive)
		Reset();
}

void Enemy::ReceiveDamage(int damage)
{
	if (m_stopTime > 0.0f)
		return;

	m_hp -= damage;
	m_stopTime = 0.2f;
	m_ivTime = 0.2f;
	m_enabled = false;

	//�G�t�F�N�g�̐ݒ�
	m_effect = NewGO<EffectEmitter>(0);
	m_effect->SetPosition(m_position);
	m_effect->Init(9);
	m_effect->Play();

	if (m_hp <= 0) {
		Dead();
	}
}

void Enemy::Direction()
{
	//�v���C���[�̕����ɉ����Č�����ς���
	if (player->GetPosition().x > m_position.x) {
		m_direction = 1;
	}
	else {
		m_direction = -1;
	}

	if (m_enemyNum == 2) {
		m_direction *= -1;
	}

	m_rotation.SetRotationYFromDirectionXZ(Vector3(m_direction, 0.0f, 0.0f));
	m_modelRender.SetRotation(m_rotation);
}

void Enemy::Render(RenderContext& rc)
{
	if (m_enabled) {
		m_modelRender.Draw(rc);
	}
}