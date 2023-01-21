#include "stdafx.h"
#include "Player.h"

#include "Game.h"
#include "GameManager.h"
#include "Barrett.h"
#include "Weapon.h"
#include "Stage.h"
#include "GameCamera.h"

#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

namespace
{
	const int JUMP_COUNT = 2;						//�W�����v�ł����
	const int MAX_HP = 15;							//�ő�HP
	const int USE_MP[5] = { 0, 3, 15, 6, 5 };		//����MP
	const float MOVESPEED = 300.0f;					//�ړ����x
	const float JUMPSPEED = 700.0f;					//�W�����v���x
	const float CLIMBSPEED = 100.0f;				//�o�鑬�x
	const float SPRINGSPEED = 1000.0f;				//�o�l�̃W�����v���x
	const float GRAVITY = 10.0f;					//�d��
	const float GRAVITYACCEL = 2.5f;				//�d�͉���
}


Player::Player()
{

}

Player::~Player()
{
	for (int i = 0; i < 4; i++) {

		DeleteGO(barrett[i]);
	}

	DeleteGO(weapon);
}

bool Player::Start()
{
	game = FindGO<Game>("game");
	gameManager = FindGO<GameManager>("gameManager");

	//�A�j���[�V�����N���b�v
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Unitychan/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Run].Load("Assets/animData/Unitychan/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/Unitychan/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	animationClips[enAnimationClip_Sliding].Load("Assets/animData/Unitychan/sliding.tka");
	animationClips[enAnimationClip_Sliding].SetLoopFlag(false);
	animationClips[enAnimationClip_Climb].Load("Assets/animData/Unitychan/climb.tka");
	animationClips[enAnimationClip_Climb].SetLoopFlag(true);
	animationClips[enAnimationClip_Climb_Idle].Load("Assets/animData/Unitychan/climb_idle.tka");
	animationClips[enAnimationClip_Climb_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Attack].Load("Assets/animData/Unitychan/attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	animationClips[enAnimationClip_Damage].Load("Assets/animData/Unitychan/damage.tka");
	animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	animationClips[enAnimationClip_IsBigin].Load("Assets/animData/Unitychan/bigin.tka");
	animationClips[enAnimationClip_IsBigin].SetLoopFlag(false);

	//�����X�e�[�^�X�̐ݒ�
	m_hp = MAX_HP;
	m_maxJumpTime = JUMP_COUNT;

	//�v���C���[���f���̐ݒ�
	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_characterController.Init(25.0f, 50.0f, m_position);
	m_modelRender.SetPosition(m_position);

	//�e�̍쐬
	for (int i = 0; i < 4; i++) {
		barrett[i] = NewGO<Barrett>(0, "barrett");
		barrett[i]->SetUser(Barrett::enUser_Player);
		barrett[i]->SetPower(m_weaponPower[0]);
		barrett[i]->SetSpeed(15.0f);
		barrett[i]->SetSoundVolume(gameManager->m_sfx);

		//�A�C�e�����擾���Ă��Ȃ��Ȃ�
		//if (!gameManager->m_getItem[i]) {
		//	m_maxJumpTime = 1;
		//}
	}

	weapon = NewGO<Weapon>(0, "weapon");
	weapon->SetUser(Weapon::enUser_Player);
	weapon->SetSoundVolume(gameManager->m_sfx);

	for (int i = 0; i < 4; i++) {
		//���킪�g�p�\���ǂ���
		//m_canUseWeapon[i + 1] = gameManager->m_isStageClear[i];
		//m_canUseWeapon[i] = m_canUseWeapon[i];
		m_weaponMP[i] = 15;

		//����摜�̈ʒu��ݒ�
		m_playerWeaponSpriteRender[i].SetPosition({ 830.0f, 290.0f, 0.0f });
		m_playerWeaponSpriteRender[i].Update();
	}

	for (int i = 0; i < 15; i++) {
		//HP�̐ݒ�
		m_playerHPSpriteRender[i].Init("Assets/sprite/hp.dds", 32.0f, 16.0f);
		m_playerHPSpriteRender[i].SetPivot(Vector2(0.5f, 0.5f));
		m_playerHPSpriteRender[i].SetPosition({ -900.0f, 260.0f + (i * 16.0f), 0.0f});
		m_playerHPSpriteRender[i].Update();

		//MP�̐ݒ�
		m_playerMPSpriteRender[i].Init("Assets/sprite/hp.dds", 32.0f, 16.0f);
		m_playerMPSpriteRender[i].SetPivot(Vector2(0.5f, 0.5f));
		m_playerMPSpriteRender[i].SetPosition({-825.0f, 260.0f + (i * 16.0f), 0.0f});
		m_playerMPSpriteRender[i].Update();
	}

	//MP�o�[�̑���
	m_playerMPBarSpriteRender.Init("Assets/sprite/hpBar.dds", 32.0f, 256.0f);
	m_playerMPBarSpriteRender.SetPosition({ -825.0f, 380.0f, 0.0f });
	m_playerMPBarSpriteRender.Update();

	//����̉摜���쐬
	m_playerWeaponSpriteRender[0].Init("Assets/sprite/weapon_0.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[1].Init("Assets/sprite/weapon_1.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[2].Init("Assets/sprite/weapon_2.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[3].Init("Assets/sprite/weapon_3.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[4].Init("Assets/sprite/weapon_0.dds", 64.0f, 64.0f);


	//�G�t�F�N�g�̐ݒ�
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/death.efk");
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/effect/bigin.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/effect/hit.efk");

	//�_���[�W���̉�
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/damage.wav");
	//���S���̉�
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/dead.wav");
	//���n���̉�
	//g_soundEngine->ResistWaveFileBank(7, "Assets/sound/damage.wav");
	//�X���C�f�B���O���̉�
	//g_soundEngine->ResistWaveFileBank(8, "Assets/sound/damage.wav");
	//�񕜎��̉�
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/heal.wav");

	//�����̐ݒ�
	m_fontRender.SetText(L"Ready...");
	m_fontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_fontRender.SetPosition(Vector3(-50.0f, 50.0f, 0.0f));
	m_fontRender.SetShadowParam(true, 2.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	//�f�o�b�O�̐ݒ�
	m_debugFontRender.SetPosition(Vector3(500.0f, 0.0f, 0.0f));

	//�G�t�F�N�g���o��
	m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(8);
	m_effect->Play();
	m_effect->SetPosition(m_position);

	//�v���C���[�̕�����ݒ�
	m_direction = 1;
	m_rotation.SetRotationYFromDirectionXZ(Vector3(m_direction, 0.0f, 0.0f));
	m_modelRender.SetRotation(m_rotation);

	return true;
}

void Player::Update()
{
	if (m_enPlayerState != enPlayerState_IsBigin &&
		m_enPlayerState != enPlayerState_Scroll &&
		m_enPlayerState != enPlayerState_Dead &&
		m_enPlayerState != enPlayerState_Clear) {

		MoveX();

		MoveY();

		if (fabsf(m_position.z) > 0.0f)
			m_characterController.SetPosition(Vector3(m_position.x, m_position.y, 0.0f));

		m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
		m_modelRender.SetPosition(m_position);

		Rotation();

		SelectWeapon();

		//�ˌ��{�^������������
		if (g_pad[0]->IsTrigger(enButtonX)) {
			Shot();
		}

		//�A�C�e���{�^������������
		if (g_pad[0]->IsTrigger(enButtonY)) {

			if (gameManager->m_leftUp > 0 && m_hp < MAX_HP) {
				//Y�ʂ��g�p����
				m_heal = 15;
				m_healDifference = 0;
				gameManager->m_leftUp -= 1;
				Font();
			}
		}

		//�񕜂ł���Ȃ�
		if (m_heal > 0) {
			if (m_frameCount % 3 == 0) {
				Heal(m_heal, m_healDifference);
			}
			m_frameCount++;
		}

		Actor::Blink();
	}

	ManageState();

	PlayAnimation();

	m_modelRender.Update();

	Debug();

}

void Player::MoveX()
{
	//x�̈ړ����x��0.0f�ɂ���
	m_moveSpeed.x = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾
	m_stickL.x = g_pad[0]->GetLStickXF();

	//�������ԂłȂ��Ȃ�
	if (IsEnableMove() == false) {

		if (m_enPlayerState == enPlayerState_Damage) {
			//�m�b�N�o�b�N����
			m_moveSpeed.x -= 200.0f * m_stopTime * m_direction;
			m_stopTime -= g_gameTime->GetFrameDeltaTime();
			return;
		}
		else if (m_enPlayerState == enPlayerState_Attack) {
			//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
			m_moveSpeed.x += m_addMoveSpeed.x;
			m_addMoveSpeed.x = 0.0f;
			return;
		}
		else {
			return;
		}
	}

	if (m_enPlayerState != enPlayerState_WallJump) {

		if (fabsf(m_stickL.x) > 0.3f) {

			if (fabsf(m_oldPos.x - m_position.x) < 0.5f) {

				m_isMove = true;

				//A�{�^���������ꂽ��
				if (g_pad[0]->IsTrigger(enButtonA)) {

					if (m_characterController.IsOnGround())
						return;

					//�W�����v������
					m_oldDirection = m_direction;
					m_jumpCount = -1;
					m_gravityAccel = 0.0f;
					m_jumpTime = 0.0f;
					m_isSliding = false;
					m_isClimb = false;
					m_enPlayerState = enPlayerState_WallJump;
				}
			}
			else {
				m_isMove = false;
			}

			m_oldPos = m_position;
		}
	}


	if (m_enPlayerState == enPlayerState_Sliding) {
		//�X���C�f�B���O����
		m_moveSpeed.x += 250.0f * m_stopTime * m_direction;
		m_stopTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed.x += m_stickL.x * MOVESPEED + m_addMoveSpeed.x;

	m_addMoveSpeed = Vector3::Zero;
}

void Player::MoveY()
{
	if (m_enPlayerState == enPlayerState_IsBigin ||
		m_enPlayerState == enPlayerState_Dead) {
		
		m_moveSpeed.y = 0.0f;
		return;
	}

	//�n�ʂɕt���Ă�����
	if (m_characterController.IsOnGround() || m_isClimb){
		//�d�͂𖳂���
		m_moveSpeed.y = 0.0f;
		m_jumpCount = 0;
		m_gravityAccel = 0.0f;
		m_jumpTime = 0.0f;
	}
	else {
		//�d�͂𔭐�������
		m_moveSpeed.y -= GRAVITY + m_gravityAccel;
		m_gravityAccel += GRAVITYACCEL;

		//�n��ŃW�����v���Ă��Ȃ��Ȃ�
		if (m_jumpCount == 0) {
			m_jumpCount++;
		}
	}
	
	//�o�l�𓥂񂾂�
	if (m_isSpring && m_jumpTime <= 0.4f) {

		m_jumpTime += g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.y = SPRINGSPEED;
		m_isSliding = false;
	}
	else {
		m_isSpring = false;
	}


	//�X���C�f�B���O�����Ă���Ȃ�
	if (m_isSliding) {
		return;
	}

	//���X�e�B�b�N�̓��͗ʂ��擾
	m_stickL.y = g_pad[0]->GetLStickYF();

	//�n�V�S�ɓo���Ȃ�
	if (m_canClimb) {

		if (m_stickL.y < -0.7f) {

			m_isClimb = true;
			m_moveSpeed.y -= CLIMBSPEED;
		}
		else if (m_stickL.y > 0.7f) {

			m_isClimb = true;
			m_moveSpeed.y += CLIMBSPEED;
		}
		else {

			if (m_isClimb && m_characterController.IsOnGround()) {

				m_isClimb = false;
			}
		}
	}
	else {

		m_isClimb = false;
	}
	
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//���X�e�B�b�N�����ɓ|���Ă���Ȃ�
		if (m_stickL.y < -0.7f && m_characterController.IsOnGround()) {
			//�X���C�f�B���O������
			m_stopTime = 2.0f;
			m_isSliding = true;
		}
		else if (m_jumpCount <= m_maxJumpTime) {
			//�W�����v������
			m_jumpCount++;
			m_gravityAccel = 0.0f;
			m_jumpTime = 0.0f;
			m_isSliding = false;
		}
		m_isClimb = false;
	}
	//A�{�^���������Ă���Ȃ�
	if (g_pad[0]->IsPress(enButtonA)) {

		if (m_jumpCount <= m_maxJumpTime && m_jumpTime <= 0.3f && m_jumpCount > 0) {
			//�W�����v����������
			m_jumpTime += g_gameTime->GetFrameDeltaTime();
			m_moveSpeed.y = JUMPSPEED + m_addMoveSpeed.y;
		}
	}
}

void Player::Rotation()
{
	if (IsEnableShot() == false) {
		return;
	}

	//������ݒ�
	if (m_stickL.x > 0.001f) {
		m_direction = 1;
	}
	else if (m_stickL.x < -0.001f) {
		m_direction = -1;
	}

	//�o�蒆�Ȃ�A��������
	if (m_isClimb) {
		if (fabsf(m_stickL.y) > 0.7f) {
			m_moveSpeed.z = 5.0f;
		}
	}
	else {
		m_moveSpeed.z = 0.0f;
	}

	//�X�e�B�b�N�̓��͂���������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		m_rotation.SetRotationYFromDirectionXZ(Vector3(m_direction, 0.0f, m_moveSpeed.z));
		m_modelRender.SetRotation(m_rotation);
	}
}

void Player::ProcessCommonStateTransition()
{
	if (m_enPlayerState == enPlayerState_Dead) {
		m_enPlayerState = enPlayerState_Dead;
	}

	if (m_isClimb) {
		//y�̈ړ����x����������
		if (fabsf(m_moveSpeed.y) >= 0.001f) {
			//�o��X�e�[�g
			m_enPlayerState = enPlayerState_Climb;
			return;
		}
		else {
			//�o��ҋ@�X�e�[�g
			m_enPlayerState = enPlayerState_Climb_Idle;
			return;
		}
	}

	//�W�����v���Ă���Ȃ�
	if (m_characterController.IsJump()) {
		//�W�����v�X�e�[�g
		m_enPlayerState = enPlayerState_Jump;
		return;
	}

	//�X���C�f�B���O���Ă���Ȃ�
	if (m_isSliding) {
		//�X���C�f�B���O�X�e�[�g
		m_enPlayerState = enPlayerState_Sliding;
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX) && m_canSelectWeapon) {
		//�U���X�e�[�g
		m_enPlayerState = enPlayerState_Attack;
		return;
	}

	//x�̈ړ����x����������
	if (fabsf(m_stickL.x) >= 0.001f){
		//�ړ��X�e�[�g
		m_enPlayerState = enPlayerState_Move;
		return;
	}
	else{
		//�ҋ@�X�e�[�g
		m_enPlayerState = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessAttackStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessSlidingStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		m_isSliding = false;
		m_stopTime = 0.0f;
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessWallJumpStateTransition()
{
	if (m_wallJumpTime > 0.5f) {
		m_wallJumpTime = 0.0f;
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
	else {
		m_wallJumpTime += g_gameTime->GetFrameDeltaTime();
		//m_addMoveSpeed.y = 600.0f;
		m_addMoveSpeed.x += 500.0f * -m_oldDirection;
	}
}

void  Player::ProcessDamageStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		m_enabled = true;
		m_stopTime = 0.0f;
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessIsBiginStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		Font();
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessScrollStateTransition()
{
	m_scrollTime += g_gameTime->GetFrameDeltaTime();

	if (m_scrollTime > 2.3f) {
		//�I�u�W�F�N�g�̐���������
		Stage* stage = FindGO<Stage>("stage");
		stage->DeleteObject(true, false, false);

		m_scrollTime = 0.0f;

		GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
		gameCamera->EnStateIdle();

		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
	else if(m_scrollTime > 1.5f) {
		m_moveSpeed = m_addMoveSpeed;
		m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);
		m_modelRender.SetPosition(m_position);
	}
}

void Player::ProcessDeadStateTransition()
{
	if (!m_effect->IsPlay()) {
		//���s�X�e�[�g�֑J�ڂ���
		game->IsGameOver(gameManager->GetPlayerLeft());
	}
}

void Player::ProcessClearStateTransition()
{
	game->IsGameClear();
}

void Player::ManageState()
{
	switch (m_enPlayerState) {
	case enPlayerState_Sliding:
		ProcessSlidingStateTransition();
		break;

	case enPlayerState_Attack:
		ProcessAttackStateTransition();
		break;

	case enPlayerState_WallJump:
		ProcessWallJumpStateTransition();
		break;

	case enPlayerState_Damage:
		ProcessDamageStateTransition();
		break;

	case enPlayerState_IsBigin:
		ProcessIsBiginStateTransition();
		break;

	case enPlayerState_Scroll:
		ProcessScrollStateTransition();
		break;

	case enPlayerState_Dead:
		ProcessDeadStateTransition();
		break;

	case enPlayerState_Clear:
		ProcessClearStateTransition();
		break;

	default:
		ProcessCommonStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	switch (m_enPlayerState)
	{
	case enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case enPlayerState_Move:
		m_modelRender.PlayAnimation(enAnimationClip_Run);
		break;

	case enPlayerState_Jump:
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;

	case enPlayerState_WallJump:
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;

	case enPlayerState_Sliding:
		m_modelRender.PlayAnimation(enAnimationClip_Sliding);
		break;

	case enPlayerState_Climb_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Climb_Idle);
		break;

	case enPlayerState_Climb:
		m_modelRender.PlayAnimation(enAnimationClip_Climb);
		break;

	case enPlayerState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack);
		break;

	case enPlayerState_Damage:
		m_modelRender.PlayAnimation(enAnimationClip_Damage);
		break;

	case enPlayerState_Clear:
		m_modelRender.PlayAnimation(enAnimationClip_Clear);
		break;

	case enPlayerState_IsBigin:
		m_modelRender.PlayAnimation(enAnimationClip_IsBigin);
		break;
	}
}

void Player::SelectWeapon()
{
	if (!m_canSelectWeapon) {
		return;
	}

	//����̑I��
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		//�z��ŉ񂵂āA�g�p�s�\�Ȃ�l�����������Ă���
		while (true) {
			m_selectWeapon--;

			if (m_selectWeapon < 0)
				m_selectWeapon = 5;

			if (m_canUseWeapon[m_selectWeapon] && m_weaponMP[m_selectWeapon] > 0)
				break;
		}
	}
	else if (g_pad[0]->IsTrigger(enButtonRight)) {
		//�z��ŉ񂵂āA�g�p�s�\�Ȃ�l��傫�����Ă���
		while (true) {
			m_selectWeapon++;
			m_selectWeapon = m_selectWeapon;

			if (m_selectWeapon > 5)
				m_selectWeapon = 0;

			if (m_canUseWeapon[m_selectWeapon] && m_weaponMP[m_selectWeapon] > 0)
				break;
		}
	}
}

void Player::Shot()
{
	//�e�����Ă��ԂłȂ��Ȃ�
	if (IsEnableShot() == false) {
		return;
	}
	weapon->SetWeaponNum(m_selectWeapon);

	//������g�p����
	switch (m_selectWeapon) {
	case 0:
		for (int i = 0; i < 4; i++) {
			//�z��ŉ񂵂�active�łȂ��Ȃ�
			if (!barrett[i]->IsActive()) {
				barrett[i]->SetPosition(m_position);
				barrett[i]->SetDirection(m_direction);
				barrett[i]->Activate();
				break;
			}
		}
		break;

	default:
		//active�łȂ����AMP������Ȃ�
		if (!weapon->IsActive() && m_weaponMP[m_selectWeapon] > 0) {
			weapon->SetPosition(m_position);
			weapon->SetDirection(m_direction);
			weapon->SetPower(m_weaponPower[m_selectWeapon]);
			weapon->Activate();
			m_weaponMP[m_selectWeapon] -= USE_MP[m_selectWeapon];
		}
		break;
	}
}

void Player::ReceiveDamage(int damage)
{
	if (m_enPlayerState == enPlayerState_Damage ||
		m_enPlayerState == enPlayerState_Scroll ||
		m_enPlayerState == enPlayerState_Dead)
		return;

	if (damage != 99) {

		//�_���[�W���󂯂Ȃ�
		if (m_ivTime > 0.0f)
			return;

		m_ivTime = 1.5f;
	}

	//HP�����炷
	m_hp -= damage;

	if (m_hp <= 0) {
		m_hp = 0;

		if (m_enabled) {
			//�_�E���X�e�[�g
			m_enPlayerState = enPlayerState_Dead;

			//�G�t�F�N�g���o��
			m_effect = NewGO<EffectEmitter>(0);
			m_effect->Init(5);
			m_effect->SetPosition(m_position);
			m_effect->Play();

			//���S�����o��
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(6);
			se->Play(false);
			se->SetVolume(m_volume);

			//�c�@�����炷
			gameManager->SetPlayerLeft(gameManager->GetPlayerLeft() - 1);
			Font();

			m_enabled = false;
		}
	}
	else {
		//�_���[�W�X�e�[�g
		m_enPlayerState = enPlayerState_Damage;

		/*
		//�G�t�F�N�g���o��
		hitEffect = NewGO<EffectEmitter>(0);
		hitEffect->Init(1);
		hitEffect->Play();
		*/

		//�_���[�W�����o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(m_volume);
	}

	//�d�����Ԃ����
	m_stopTime = 2.0f;
	m_jumpTime = 1.0f;
	m_moveSpeed.z = 0.0f;

	//�A�N�V�����̕ύX
	m_isClimb = false;
	m_isSliding = false;
}

void Player::Heal(int heal, bool difference)
{
	m_heal = heal;
	m_healDifference = difference;

	//�񕜗ʂ�0�ɂȂ�����
	if ((difference == 0 && m_hp >= MAX_HP) || (difference == 1 && m_weaponMP[m_selectWeapon] >= 15)) {
		m_heal = 0;
		return;
	}

	//��ނ�HP�Ȃ�
	if (difference == 0) {
		m_hp++;
	}
	//��ނ�MP�Ȃ�
	else {
		m_weaponMP[m_selectWeapon]++;
	}

	//�񕜗ʂ�1����
	m_heal--;
	
	//�񕜉����o��
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(9);
	se->Play(false);
	se->SetVolume(m_volume);
}

void Player::Font()
{
	wchar_t text[255];
	swprintf_s(text, 255, L"x%d \n\nx%d", gameManager->GetPlayerLeft(), gameManager->m_leftUp);
	m_fontRender.SetPosition(Vector3(875.0f, 470.0f, 0.0f));
	m_fontRender.SetText(text);
}

void Player::Render(RenderContext& rc)
{
	//�L���Ȃ�
	if (m_enabled) {
		m_modelRender.Draw(rc);
	}

	m_fontRender.Draw(rc);

	//�J�n���̃X�e�[�g�Ȃ�
	if (m_enPlayerState == enPlayerState_IsBigin) {
		return;
	}

	//HP�̕`��
	for (int i = 0; i < m_hp; i++) {
		m_playerHPSpriteRender[i].Draw(rc);
	}

	//MP�̕`��
	if (m_selectWeapon != 0) {

		m_playerMPBarSpriteRender.Draw(rc);

		for (int j = 0; j < m_weaponMP[m_selectWeapon]; j++) {
			m_playerMPSpriteRender[j].Draw(rc);
		}
	}

	//����摜�̕`��
	m_playerWeaponSpriteRender[m_selectWeapon].Draw(rc);

	m_debugFontRender.Draw(rc);
}

void Player::Debug()
{
	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"X:%.2f \nY:%.2f \ncanWallJump:%d \noldPosition:%.2f \n%d", m_position.x, m_position.y, m_isMove, m_oldPos.x, m_enPlayerState);
	m_debugFontRender.SetText(debugText);

	if (g_pad[0]->IsTrigger(enButtonB)) {

		m_hp -= 1;
	}
}