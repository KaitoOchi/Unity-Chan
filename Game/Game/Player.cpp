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
	const int JUMP_COUNT = 2;						//ジャンプできる回数
	const int MAX_HP = 15;							//最大HP
	const int USE_MP[5] = { 0, 3, 15, 6, 5 };		//消費MP
	const float MOVESPEED = 300.0f;					//移動速度
	const float JUMPSPEED = 700.0f;					//ジャンプ速度
	const float CLIMBSPEED = 100.0f;				//登る速度
	const float SPRINGSPEED = 1000.0f;				//バネのジャンプ速度
	const float GRAVITY = 10.0f;					//重力
	const float GRAVITYACCEL = 2.5f;				//重力加速
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

	//アニメーションクリップ
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

	//初期ステータスの設定
	m_hp = MAX_HP;
	m_maxJumpTime = JUMP_COUNT;

	//プレイヤーモデルの設定
	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	m_characterController.Init(25.0f, 50.0f, m_position);
	m_modelRender.SetPosition(m_position);

	//弾の作成
	for (int i = 0; i < 4; i++) {
		barrett[i] = NewGO<Barrett>(0, "barrett");
		barrett[i]->SetUser(Barrett::enUser_Player);
		barrett[i]->SetPower(m_weaponPower[0]);
		barrett[i]->SetSpeed(15.0f);
		barrett[i]->SetSoundVolume(gameManager->m_sfx);

		//アイテムを取得していないなら
		//if (!gameManager->m_getItem[i]) {
		//	m_maxJumpTime = 1;
		//}
	}

	weapon = NewGO<Weapon>(0, "weapon");
	weapon->SetUser(Weapon::enUser_Player);
	weapon->SetSoundVolume(gameManager->m_sfx);

	for (int i = 0; i < 4; i++) {
		//武器が使用可能かどうか
		//m_canUseWeapon[i + 1] = gameManager->m_isStageClear[i];
		//m_canUseWeapon[i] = m_canUseWeapon[i];
		m_weaponMP[i] = 15;

		//武器画像の位置を設定
		m_playerWeaponSpriteRender[i].SetPosition({ 830.0f, 290.0f, 0.0f });
		m_playerWeaponSpriteRender[i].Update();
	}

	for (int i = 0; i < 15; i++) {
		//HPの設定
		m_playerHPSpriteRender[i].Init("Assets/sprite/hp.dds", 32.0f, 16.0f);
		m_playerHPSpriteRender[i].SetPivot(Vector2(0.5f, 0.5f));
		m_playerHPSpriteRender[i].SetPosition({ -900.0f, 260.0f + (i * 16.0f), 0.0f});
		m_playerHPSpriteRender[i].Update();

		//MPの設定
		m_playerMPSpriteRender[i].Init("Assets/sprite/hp.dds", 32.0f, 16.0f);
		m_playerMPSpriteRender[i].SetPivot(Vector2(0.5f, 0.5f));
		m_playerMPSpriteRender[i].SetPosition({-825.0f, 260.0f + (i * 16.0f), 0.0f});
		m_playerMPSpriteRender[i].Update();
	}

	//MPバーの装飾
	m_playerMPBarSpriteRender.Init("Assets/sprite/hpBar.dds", 32.0f, 256.0f);
	m_playerMPBarSpriteRender.SetPosition({ -825.0f, 380.0f, 0.0f });
	m_playerMPBarSpriteRender.Update();

	//武器の画像を作成
	m_playerWeaponSpriteRender[0].Init("Assets/sprite/weapon_0.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[1].Init("Assets/sprite/weapon_1.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[2].Init("Assets/sprite/weapon_2.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[3].Init("Assets/sprite/weapon_3.dds", 64.0f, 64.0f);
	m_playerWeaponSpriteRender[4].Init("Assets/sprite/weapon_0.dds", 64.0f, 64.0f);


	//エフェクトの設定
	EffectEngine::GetInstance()->ResistEffect(5, u"Assets/effect/death.efk");
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/effect/bigin.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/effect/hit.efk");

	//ダメージ時の音
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/damage.wav");
	//死亡時の音
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/dead.wav");
	//着地時の音
	//g_soundEngine->ResistWaveFileBank(7, "Assets/sound/damage.wav");
	//スライディング時の音
	//g_soundEngine->ResistWaveFileBank(8, "Assets/sound/damage.wav");
	//回復時の音
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/heal.wav");

	//文字の設定
	m_fontRender.SetText(L"Ready...");
	m_fontRender.SetPivot(Vector2(0.5f, 0.5f));
	m_fontRender.SetPosition(Vector3(-50.0f, 50.0f, 0.0f));
	m_fontRender.SetShadowParam(true, 2.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	//デバッグの設定
	m_debugFontRender.SetPosition(Vector3(500.0f, 0.0f, 0.0f));

	//エフェクトを出す
	m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(8);
	m_effect->Play();
	m_effect->SetPosition(m_position);

	//プレイヤーの方向を設定
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

		//射撃ボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonX)) {
			Shot();
		}

		//アイテムボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonY)) {

			if (gameManager->m_leftUp > 0 && m_hp < MAX_HP) {
				//Y缶を使用する
				m_heal = 15;
				m_healDifference = 0;
				gameManager->m_leftUp -= 1;
				Font();
			}
		}

		//回復できるなら
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
	//xの移動速度を0.0fにする
	m_moveSpeed.x = 0.0f;

	//左スティックの入力量を取得
	m_stickL.x = g_pad[0]->GetLStickXF();

	//動ける状態でないなら
	if (IsEnableMove() == false) {

		if (m_enPlayerState == enPlayerState_Damage) {
			//ノックバックする
			m_moveSpeed.x -= 200.0f * m_stopTime * m_direction;
			m_stopTime -= g_gameTime->GetFrameDeltaTime();
			return;
		}
		else if (m_enPlayerState == enPlayerState_Attack) {
			//移動速度にスティックの入力量を加算する。
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

				//Aボタンが押されたら
				if (g_pad[0]->IsTrigger(enButtonA)) {

					if (m_characterController.IsOnGround())
						return;

					//ジャンプさせる
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
		//スライディングする
		m_moveSpeed.x += 250.0f * m_stopTime * m_direction;
		m_stopTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	//移動速度にスティックの入力量を加算する。
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

	//地面に付いていたら
	if (m_characterController.IsOnGround() || m_isClimb){
		//重力を無くす
		m_moveSpeed.y = 0.0f;
		m_jumpCount = 0;
		m_gravityAccel = 0.0f;
		m_jumpTime = 0.0f;
	}
	else {
		//重力を発生させる
		m_moveSpeed.y -= GRAVITY + m_gravityAccel;
		m_gravityAccel += GRAVITYACCEL;

		//地上でジャンプしていないなら
		if (m_jumpCount == 0) {
			m_jumpCount++;
		}
	}
	
	//バネを踏んだら
	if (m_isSpring && m_jumpTime <= 0.4f) {

		m_jumpTime += g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.y = SPRINGSPEED;
		m_isSliding = false;
	}
	else {
		m_isSpring = false;
	}


	//スライディングをしているなら
	if (m_isSliding) {
		return;
	}

	//左スティックの入力量を取得
	m_stickL.y = g_pad[0]->GetLStickYF();

	//ハシゴに登れるなら
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
	
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA)) {

		//左スティックを下に倒しているなら
		if (m_stickL.y < -0.7f && m_characterController.IsOnGround()) {
			//スライディングをする
			m_stopTime = 2.0f;
			m_isSliding = true;
		}
		else if (m_jumpCount <= m_maxJumpTime) {
			//ジャンプさせる
			m_jumpCount++;
			m_gravityAccel = 0.0f;
			m_jumpTime = 0.0f;
			m_isSliding = false;
		}
		m_isClimb = false;
	}
	//Aボタンを押しているなら
	if (g_pad[0]->IsPress(enButtonA)) {

		if (m_jumpCount <= m_maxJumpTime && m_jumpTime <= 0.3f && m_jumpCount > 0) {
			//ジャンプさせ続ける
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

	//方向を設定
	if (m_stickL.x > 0.001f) {
		m_direction = 1;
	}
	else if (m_stickL.x < -0.001f) {
		m_direction = -1;
	}

	//登り中なら、奥を向く
	if (m_isClimb) {
		if (fabsf(m_stickL.y) > 0.7f) {
			m_moveSpeed.z = 5.0f;
		}
	}
	else {
		m_moveSpeed.z = 0.0f;
	}

	//スティックの入力があったら
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
		//yの移動速度があったら
		if (fabsf(m_moveSpeed.y) >= 0.001f) {
			//登りステート
			m_enPlayerState = enPlayerState_Climb;
			return;
		}
		else {
			//登り待機ステート
			m_enPlayerState = enPlayerState_Climb_Idle;
			return;
		}
	}

	//ジャンプしているなら
	if (m_characterController.IsJump()) {
		//ジャンプステート
		m_enPlayerState = enPlayerState_Jump;
		return;
	}

	//スライディングしているなら
	if (m_isSliding) {
		//スライディングステート
		m_enPlayerState = enPlayerState_Sliding;
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX) && m_canSelectWeapon) {
		//攻撃ステート
		m_enPlayerState = enPlayerState_Attack;
		return;
	}

	//xの移動速度があったら
	if (fabsf(m_stickL.x) >= 0.001f){
		//移動ステート
		m_enPlayerState = enPlayerState_Move;
		return;
	}
	else{
		//待機ステート
		m_enPlayerState = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessAttackStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessSlidingStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		m_isSliding = false;
		m_stopTime = 0.0f;
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void  Player::ProcessWallJumpStateTransition()
{
	if (m_wallJumpTime > 0.5f) {
		m_wallJumpTime = 0.0f;
		//他のステートに遷移する
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
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessIsBiginStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation()) {
		Font();
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessScrollStateTransition()
{
	m_scrollTime += g_gameTime->GetFrameDeltaTime();

	if (m_scrollTime > 2.3f) {
		//オブジェクトの生成をする
		Stage* stage = FindGO<Stage>("stage");
		stage->DeleteObject(true, false, false);

		m_scrollTime = 0.0f;

		GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
		gameCamera->EnStateIdle();

		//他のステートに遷移する
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
		//失敗ステートへ遷移する
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

	//武器の選択
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		//配列で回して、使用不可能なら値を小さくしていく
		while (true) {
			m_selectWeapon--;

			if (m_selectWeapon < 0)
				m_selectWeapon = 5;

			if (m_canUseWeapon[m_selectWeapon] && m_weaponMP[m_selectWeapon] > 0)
				break;
		}
	}
	else if (g_pad[0]->IsTrigger(enButtonRight)) {
		//配列で回して、使用不可能なら値を大きくしていく
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
	//弾を撃てる状態でないなら
	if (IsEnableShot() == false) {
		return;
	}
	weapon->SetWeaponNum(m_selectWeapon);

	//武器を使用する
	switch (m_selectWeapon) {
	case 0:
		for (int i = 0; i < 4; i++) {
			//配列で回してactiveでないなら
			if (!barrett[i]->IsActive()) {
				barrett[i]->SetPosition(m_position);
				barrett[i]->SetDirection(m_direction);
				barrett[i]->Activate();
				break;
			}
		}
		break;

	default:
		//activeでないかつ、MPがあるなら
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

		//ダメージを受けない
		if (m_ivTime > 0.0f)
			return;

		m_ivTime = 1.5f;
	}

	//HPを減らす
	m_hp -= damage;

	if (m_hp <= 0) {
		m_hp = 0;

		if (m_enabled) {
			//ダウンステート
			m_enPlayerState = enPlayerState_Dead;

			//エフェクトを出す
			m_effect = NewGO<EffectEmitter>(0);
			m_effect->Init(5);
			m_effect->SetPosition(m_position);
			m_effect->Play();

			//死亡音を出す
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(6);
			se->Play(false);
			se->SetVolume(m_volume);

			//残機を減らす
			gameManager->SetPlayerLeft(gameManager->GetPlayerLeft() - 1);
			Font();

			m_enabled = false;
		}
	}
	else {
		//ダメージステート
		m_enPlayerState = enPlayerState_Damage;

		/*
		//エフェクトを出す
		hitEffect = NewGO<EffectEmitter>(0);
		hitEffect->Init(1);
		hitEffect->Play();
		*/

		//ダメージ音を出す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(5);
		se->Play(false);
		se->SetVolume(m_volume);
	}

	//硬直時間を作る
	m_stopTime = 2.0f;
	m_jumpTime = 1.0f;
	m_moveSpeed.z = 0.0f;

	//アクションの変更
	m_isClimb = false;
	m_isSliding = false;
}

void Player::Heal(int heal, bool difference)
{
	m_heal = heal;
	m_healDifference = difference;

	//回復量が0になったら
	if ((difference == 0 && m_hp >= MAX_HP) || (difference == 1 && m_weaponMP[m_selectWeapon] >= 15)) {
		m_heal = 0;
		return;
	}

	//種類がHPなら
	if (difference == 0) {
		m_hp++;
	}
	//種類がMPなら
	else {
		m_weaponMP[m_selectWeapon]++;
	}

	//回復量を1減少
	m_heal--;
	
	//回復音を出す
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
	//有効なら
	if (m_enabled) {
		m_modelRender.Draw(rc);
	}

	m_fontRender.Draw(rc);

	//開始時のステートなら
	if (m_enPlayerState == enPlayerState_IsBigin) {
		return;
	}

	//HPの描画
	for (int i = 0; i < m_hp; i++) {
		m_playerHPSpriteRender[i].Draw(rc);
	}

	//MPの描画
	if (m_selectWeapon != 0) {

		m_playerMPBarSpriteRender.Draw(rc);

		for (int j = 0; j < m_weaponMP[m_selectWeapon]; j++) {
			m_playerMPSpriteRender[j].Draw(rc);
		}
	}

	//武器画像の描画
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