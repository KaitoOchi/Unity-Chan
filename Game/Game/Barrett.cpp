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

	//コリジョンの設定
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 10.0f));
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetIsEnable(false);

	//エフェクトの設定
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/barrett.efk");

	switch (m_enUser) {
	case enUser_Player:
		//コリジョンの名前を変更
		m_collisionObject->SetName("barrett_player");
		break;

	case enUser_Enemy:
		//コリジョンの名前を変更
		m_collisionObject->SetName("barrett_enemy");
		break;

	case enUser_Boss:
		//コリジョンの名前を変更
		m_collisionObject->SetName("barrett_enemy");
		break;
	}

	//射撃時の音
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

	//発射時に一回だけ
	if (!m_shot) {

		if (m_enUser == enUser_Player) {
			//出ている間は武器を変更できないようにする
			player->SetCanSelectWeapon(false);
		}

		if (m_canChase) {
			//プレイヤーから敵に向かうベクトルを計算
			m_diff = player->GetPosition() - m_position;
			//正規化をする
			m_diff.Normalize();
		}

		m_position.y += 30.0f;

		//エフェクトの再生
		m_barrettEffect = NewGO<EffectEmitter>(0);
		m_barrettEffect->Init(0);
		m_barrettEffect->Play();

		//射撃音を出す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(0);
		se->Play(false);
		se->SetVolume(m_volume);

		//当たり判定の有効化
		m_collisionObject->SetIsEnable(true);

		m_shot = true;
	}

	//再生が完了したら
	if (!m_barrettEffect->IsPlay()) {
		//リセットする
		Reset();
		return;
	}

	//移動させる
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
		//弾のコリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_enemy");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとコリジョンが衝突したら
			if (collision->IsHit(m_collisionObject)) {
				m_isRepel = true;
				return;
			}
		}
	}
		break;

	case enUser_Enemy:
		//コリジョンとキャラコンが衝突したら
		if (m_collisionObject->IsHit(player->GetCharacterController())) {
			player->ReceiveDamage(m_power);
		}

		{
			//弾のコリジョンを取得する
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
			//コリジョンの配列をfor文で回す
			for (auto collision : collisions)
			{
				//コリジョンとコリジョンが衝突したら
				if (collision->IsHit(m_collisionObject)) {
					m_isRepel = true;
					return;
				}
			}
		}
		break;

	case enUser_Boss:
		//コリジョンとコリジョンが衝突したら
		if (m_collisionObject->IsHit(player->GetCharacterController())) {
			player->ReceiveDamage(m_power);
		}

		{
			//弾のコリジョンを取得する
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("weapon_player");
			//コリジョンの配列をfor文で回す
			for (auto collision : collisions)
			{
				//コリジョンとコリジョンが衝突したら
				if (collision->IsHit(m_collisionObject)) {
					m_isRepel = true;
					return;
				}
			}
		}
		break;
	}

	{
		//ボックスのコリジョンを取得する
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("box");
		//コリジョンの配列をfor文で回す
		for (auto collision : collisions)
		{
			//コリジョンとコリジョンが衝突したら
			if (collision->IsHit(m_collisionObject)) {
				Reset();
				return;
			}
		}
	}

	/*予定
	//コリジョンとステージが衝突したら
	if (m_collisionObject->IsHit(stage->GetPhysicsStaticObject()))
	{
		//リセットする
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