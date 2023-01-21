#include "stdafx.h"
#include "Boss.h"

#include "Player.h"
#include "Barrett.h"
#include "Weapon.h"
#include "Game.h"
#include "GameManager.h"


Boss::Boss()
{

}

Boss::~Boss()
{
	if (barrett[0] != nullptr) {
		for (int i = 0; i < 4; i++) {
			DeleteGO(barrett[i]);
		}
	}

	DeleteGO(m_collisionObject);
	DeleteGO(weapon);
}

bool Boss::Start()
{
	player = FindGO<Player>("player");

	GameManager* gameManager = FindGO<GameManager>("gameManager");
	m_volume = gameManager->m_sfx;

	m_firstPos = m_position;
	m_firstPos.x -= 400.0f;
	m_modelRender.SetPosition(m_position);
	m_rotation.SetRotationYFromDirectionXZ(Vector3(m_direction, 0.0f, 0.0f));
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

	m_hp = 15;

	m_modelRender.SetRaytracingWorld(false);
	switch (m_bossNum) {

	case 1:
		//アニメーションクリップ
		m_animationClip[enAnimationClip_Idle].Load("Assets/animData/Unitychan/idle.tka");
		m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
		m_animationClip[enAnimationClip_Walk].Load("Assets/animData/Unitychan/run.tka");
		m_animationClip[enAnimationClip_Walk].SetLoopFlag(true);
		m_animationClip[enAnimationClip_Jump].Load("Assets/animData/Unitychan/jump.tka");
		m_animationClip[enAnimationClip_Jump].SetLoopFlag(false);
		m_animationClip[enAnimationClip_Attack].Load("Assets/animData/Unitychan/attack.tka");
		m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);

		//プレイヤーモデルの設定
		m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClip, enAnimationClip_Num, enModelUpAxisY);

		//ステータスの設定
		SetStatus(0, 3, 90, 7.0f, 13.0f, 1.0f, Vector3(70.0f, 70.0f, 70.0f));

		m_atkT = 0.15f;

		//弾の作成
		for (int i = 0; i < 4; i++) {
			barrett[i] = NewGO<Barrett>(0, "barrett");
			barrett[i]->SetUser(Barrett::enUser_Boss);
			barrett[i]->SetPower(m_damage);
			barrett[i]->SetSpeed(15.0f);
		}

		weapon = NewGO<Weapon>(0, "weapon");
		weapon->SetUser(Weapon::enUser_Boss);
		break;
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		//モデルの設定
		m_modelRender.Init("Assets/modelData/enemy/boss1/boss1.tkm");

		//ステータスの設定
		SetStatus(2, 2, 70, 7.0f, 13.0f, 1.5f, Vector3(70.0f, 70.0f, 70.0f));

		m_atkT = 0.9f;

		barrett[0] = NewGO<Barrett>(0, "barrett");
		barrett[0]->SetUser(Barrett::enUser_Boss);
		barrett[0]->SetPower(m_damage);
		barrett[0]->SetSpeed(20.0f);

		m_position.y += 500.0f;
		break;
	}

	//HPバーの装飾
	m_hpBarSpriteRender.Init("Assets/sprite/hpBar.dds", 32.0f, 256.0f);
	m_hpBarSpriteRender.SetPosition({ -775.0f, 380.0f, 0.0f });
	m_hpBarSpriteRender.Update();

	for (int i = 0; i < 15; i++) {
		//HPの設定
		m_hpSpriteRender[i].Init("Assets/sprite/hp.dds", 32.0f, 16.0f);
		m_hpSpriteRender[i].SetPivot(Vector2(0.5f, 0.5f));
		m_hpSpriteRender[i].SetPosition({ -775.0f, 260.0f + (i * 16.0f), 0.0f });
		m_hpSpriteRender[i].Update();
	}

	//コリジョンの設定
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, m_colScale);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("boss");
	return true;
}

void Boss::Update()
{
	if (m_bossState != enBossState_Dead &&
		m_bossState != enBossState_Idle) {

		Move();

		Attack();

		Collision();

		Actor::Blink();
	}

	if (m_stopTime > 0.0f) {
		m_stopTime -= g_gameTime->GetFrameDeltaTime();
	}

	if (m_clearTime > 0.0f) {
		m_clearTime -= g_gameTime->GetFrameDeltaTime();

		if (m_clearTime < 0.1f) {
			m_clearTime = 0.0f;
			Game* game = FindGO<Game>("game");
			game->IsGameClear();
		}
	}

	ManageState();

	PlayAnimation();

	m_modelRender.Update();


	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"X:%.2f \nY:%.2f \nanimT:%.2f \nS:%d \nA:%d", m_position.x, m_position.y, m_animationTime, m_bossState, m_enabled);
	m_debugFontRender.SetText(debugText);
}

void Boss::Move()
{
	//範囲外に出たなら
	if (fabsf(m_position.x - m_firstPos.x) > 700.0f) {
		//反転させる
		m_direction *= -1;
		m_rotation.SetRotationYFromDirectionXZ(Vector3(m_direction, 0.0f, 0.0f));
		m_modelRender.SetRotation(m_rotation);
	}

	Vector3 moveSpeed;
	Vector3 oldPlayerPos;
	Vector3 addMoveSpeed;

	moveSpeed.x = m_speed * m_direction;

	switch (m_bossNum) {
	case 1:
		//ジャンプ中なら
		if (m_isJump) {

			moveSpeed.y = m_gravity + m_jumpSpeed;
			m_gravity -= 0.3f;

			if (m_position.y < m_firstPos.y) {
				m_isJump = false;
				m_gravity = 0.0f;
				m_position.y = m_firstPos.y;
			}
		}
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		//特殊攻撃中なら
		if (m_bossState == enBossState_SpecialAttack) {

			if (!m_action) {
				oldPlayerPos = player->GetPosition();
				m_action = true;
			}

			addMoveSpeed = oldPlayerPos - m_position;
			addMoveSpeed.Normalize();

			moveSpeed.x = (addMoveSpeed.x * m_speed) * -m_direction;
			moveSpeed.y = (m_animationTime - 0.75f) * 27.5f;
		}
		break;
	}

	m_position += moveSpeed;

	m_modelRender.SetPosition(m_position);
	m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 30.0f, 0.0f));
}

void Boss::Attack()
{
	//攻撃モーションでなければ
	if (m_bossState != enBossState_Attack &&
		m_bossState != enBossState_SpecialAttack &&
		m_bossState != enBossState_Jump) {
		return;
	}

	if (m_bossNum == 1) {
		//目の前にプレイヤーがいないなら
		if ((m_direction == -1 && m_position.x < player->GetPosition().x) ||
			(m_direction == 1 && m_position.x > player->GetPosition().x)) {
			return;
		}
	}

	m_attackTime += g_gameTime->GetFrameDeltaTime();

	if (m_attackTime > m_atkT) {

		switch (m_bossNum) {
		//Shadow
		case 1:
			if (m_bossState == enBossState_Attack ||
				m_bossState == enBossState_Jump) {

				if (m_bossNum == 1) {

					for (int i = 0; i < 4; i++) {

						//配列で回してactiveでないなら
						if (!barrett[i]->IsActive()) {
							barrett[i]->SetPosition(m_position);
							barrett[i]->SetDirection(m_direction);
							barrett[i]->Activate();

							if (m_bossState == enBossState_Jump) {
								barrett[i]->SetChase(true);
							}
							else {
								barrett[i]->SetChase(true);
							}
							break;
						}
					}
				}
			}
			else {
				//activeでないなら
				if (!weapon->IsActive()) {

					int randNum = rand() % 3 + 1;
					weapon->SetWeaponNum(randNum);
					weapon->SetPosition(m_position);
					weapon->SetDirection(m_direction);
					weapon->SetPower(m_damage);
					weapon->Activate();
				}
			}
			break;

		//UFO
		case 5:
			if (m_bossState == enBossState_Attack) {
				barrett[0]->SetPosition(m_position);
				barrett[0]->SetDirection(m_direction);
				barrett[0]->SetChase(true);
				barrett[0]->Activate();

				//射撃音を出す
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(0);
				se->Play(false);
				se->SetVolume(m_volume);
			}
			break;
		}

		m_attackTime = 0.0f;
	}
}

void Boss::Collision()
{
	//プレイヤーと接触したら
	if (m_collisionObject->IsHit(player->GetCharacterController())) {
		//ダメージを与える
		player->ReceiveDamage(m_damage);
	}

	{
		//弾のコリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("barrett_player");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとコリジョン衝突したら
			if (collision->IsHit(m_collisionObject)) {
				ReceiveDamage(player->GetWeaponPower());
			}
		}
	}

	{
		//弾のコリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとコリジョン衝突したら
			if (collision->IsHit(m_collisionObject)) {
				ReceiveDamage(player->GetWeaponPower());
			}
		}
	}
}

void Boss::ProcessCommonStateTransition()
{
	if (m_bossNum == 1) {

		if (CanJump()) {
			enBossState_Jump;

			if (!m_isJump)
				m_isJump = true;
			return;
		}
	}

	//攻撃可能なら
	if (CanAttack()) {

		int randNum = rand() % 100;
		
		if (randNum < m_atkPercent) {
			//通常攻撃
			m_bossState = enBossState_Attack;
		}
		else {
			//特殊攻撃
			m_bossState = enBossState_SpecialAttack;
		}

		enBossState_Attack;

		return;
	}

	m_bossState = enBossState_Move;
}

void Boss::ProcessMoveStateTransition()
{
	//攻撃可能またはジャンプ可能なら
	if (CanAttack() || CanJump())
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessAttackStateTransition()
{
	m_animationTime += g_gameTime->GetFrameDeltaTime();

	if (m_animationTime > 1.0f) {
		m_animationTime = 0.0f;
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessSpecialAttackStateTransition()
{
	m_animationTime += g_gameTime->GetFrameDeltaTime();

	if (m_animationTime > m_animT){
		m_animationTime = 0.0f;
		m_action = false;
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Boss::ProcessDeadStateTransition()
{
	if (!m_effect->IsPlay()) {
		return;
	}
}

void Boss::ManageState()
{
	switch (m_bossState) {

	case enBossState_Move:
		ProcessMoveStateTransition();
		break;

	case enBossState_Attack:
		ProcessAttackStateTransition();
		break;

	case enBossState_SpecialAttack:
		ProcessSpecialAttackStateTransition();
		break;

	case enBossState_Dead:
		ProcessDeadStateTransition();
		break;

	default:
		ProcessCommonStateTransition();
		break;
	}
}

void Boss::PlayAnimation()
{
	switch (m_bossState) {

	case enBossState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;

	case enBossState_Move:
		m_modelRender.PlayAnimation(enAnimationClip_Walk);
		break;

	case enBossState_Jump:
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;

	case enBossState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack);
		break;
	}
}

void Boss::ReceiveDamage(const int damage)
{
	if (m_stopTime > 0.0f)
		return;

	m_hp -= damage + m_receiveDamage;
	m_ivTime = 1.0f;

	//エフェクトの設定
	m_effect = NewGO<EffectEmitter>(0);
	m_effect->SetPosition(m_position);

	//HPが0なら
	if (m_hp <= 0) {
		m_ivTime = 0.0f;
		m_clearTime = 3.0f;
		m_collisionObject->SetIsEnable(false);
		m_bossState = enBossState_Dead;

		//エフェクトを出す
		m_effect->Init(5);
		m_effect->Play();

		//死亡音を出す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(6);
		se->Play(false);
		se->SetVolume(m_volume);
	}
	else {
		//エフェクトを出す
		m_effect->Init(9);
		m_effect->Play();
	}

	m_stopTime = 0.3f;
	m_enabled = false;
}

bool Boss::CanAttack()
{
	if (m_bossNum != 1)
		return true;

	//プレイヤーから敵に向かうベクトルを計算
	Vector3 diff = player->GetPosition() - m_position;
	//ベクトルの長さが小さかったら
	if (diff.Length() < 600.0f){
		return true;
	}
	return false;
}

bool Boss::CanJump()
{
	//プレイヤーの方が高いなら
	if ((player->GetPosition().y - m_position.y) > 200.0f) {
		return true;
	}
	return false;
}

void Boss::Render(RenderContext& rc)
{
	if(m_enabled)
		m_modelRender.Draw(rc);

	m_hpBarSpriteRender.Draw(rc);

	//HPの描画
	for (int i = 0; i < m_hp; i++) {
		m_hpSpriteRender[i].Draw(rc);
	}

	//m_debugFontRender.Draw(rc);
}