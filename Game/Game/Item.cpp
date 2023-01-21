#include "stdafx.h"
#include "Item.h"

#include "Player.h"
#include "GameManager.h"

#include "graphics/effect/EffectEmitter.h"

namespace
{
	const int HEAL_SMALL = 3;
	const int HEAL_BIG = 8;
}

Item::Item()
{

}

Item::~Item()
{
}

bool Item::Start()
{
	player = FindGO<Player>("player");
	GameManager* gameManager = FindGO<GameManager>("gameManager");
	m_volume = gameManager->m_sfx;

	//エフェクトの設定
	m_itemEffect = NewGO<EffectEmitter>(0);

	//0～99で設定
	int randNum = rand() % 100;

	//ランダムにアイテムを出現させる
	if (randNum >= 0 && randNum <= 9) {

		//回復する種類の設定
		if (randNum >= 0 && randNum <= 4){
			//エフェクトの設定
			EffectEngine::GetInstance()->ResistEffect(6, u"Assets/effect/healHP_small.efk");
			m_itemEffect->Init(6);
			m_difference = 0;
		}
		else {
			//エフェクトの設定
			EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/healMP_small.efk");
			m_itemEffect->Init(10);
			m_difference = 1;
		}

		//回復量の設定
		if (randNum % 4 == 0) {
			m_heal = HEAL_BIG;
			m_itemEffect->SetScale({ 2.0f, 2.0f, 2.0f });
		}
		else {
			m_heal = HEAL_SMALL;
		}
	}
	else if(randNum >= 10 && randNum <= 11) {
		//エフェクトの設定
		EffectEngine::GetInstance()->ResistEffect(7, u"Assets/effect/leftUp.efk");
		m_itemEffect->Init(7);
		m_itemEffect->SetScale({ 4.0f, 4.0f, 4.0f });
		m_difference = 2;
	}
	else {
		DeleteGO(this);
		return true;
	}

	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/getItem.wav");

	m_itemEffect->SetPosition(Vector3(m_position.x, m_position.y + 50.0f, m_position.z));
	m_itemEffect->Play();

	return true;
}

void Item::Update()
{
	//再生が完了したら
	if (!m_itemEffect->IsPlay()) {
		DeleteGO(this);
	}

	//プレイヤーから敵に向かうベクトルを計算。
	Vector3 diff = player->GetPosition() - m_position;

	//ベクトルの長さが小さかったら。
	if (diff.Length() <= 60.0f) {

		if (m_difference == 2) {
			//残機を設定
			GameManager* gameManager = FindGO<GameManager>("gameManager");
			gameManager->SetPlayerLeft(gameManager->GetPlayerLeft() + 1);
			player->Font();
		}
		else {
			//回復量を設定
			player->Heal(m_heal, m_difference);
		}

		//取得音の再生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(23);
		se->SetVolume(m_volume);
		se->Play(false);

		//エフェクトを停止
		m_itemEffect->Stop();
		DeleteGO(this);
	}
	//プレイヤーとの距離が離れたら
	else if (diff.Length() >= 1000.0f) {
		//エフェクトを停止
		m_itemEffect->Stop();
		DeleteGO(this);
	}
}