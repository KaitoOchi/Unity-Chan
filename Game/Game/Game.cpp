#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "GameCamera.h"
#include "Stage.h"
#include "Fade.h"

#include "GameManager.h"


Game::Game()
{
	//フレームレートを固定
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);

	//乱数シードの初期化
	srand(time(NULL));

	//当たり判定を有効化
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

Game::~Game()
{
	const auto& stage = FindGO<Stage>("stage");
	stage->DeleteObject(false, m_isClear, m_isGameOver);

	const auto& player = FindGO<Player>("player");
	DeleteGO(player);

	const auto& gameCamera = FindGO<GameCamera>("gameCamera");
	DeleteGO(gameCamera);
	/*
	//データの書き換え
	for (int i = 0; i < 4; i++) {
		m_saveData[i] = gameManager->m_isStageClear[i];
		m_saveData[i + 4] = gameManager->m_getItem[i];
	}
	m_saveData[8] = gameManager->m_leftUp;
	m_saveData[9] = gameManager->m_isOwata;
	m_saveData[10] = gameManager->m_bgm;
	m_saveData[11] = gameManager->m_sfx;
	*/
	gameManager->m_getYkan = false;
	/*
	//セーブする
	FILE* fp2 = fopen("saveData.bin", "wb");
	fwrite(m_saveData, sizeof(m_saveData), 1, fp2);
	fclose(fp2);
	*/
}

bool Game::Start()
{
	gameManager = FindGO<GameManager>("gameManager");
	//m_saveData[10] = gameManager->m_bgm;
	//m_saveData[11] = gameManager->m_sfx;

	//ゲームシーン遷移時に必要なクラスをNewGO
	NewGO<GameCamera>(0, "gameCamera");

	Stage* stage = NewGO<Stage>(0, "stage");
	stage->SetStageNum(gameManager->GetStageNum());
	stage->SetCourceNum(gameManager->GetCourceNum());

	Player* player = NewGO<Player>(1, "player");
	player->Actor::SetPosition(gameManager->GetContinuePosition());
	player->Actor::SetSoundVolume(gameManager->m_sfx);


	//HPバーの装飾
	m_playerHPBarSpriteRender.Init("Assets/sprite/hpBar.dds", 32.0f, 256.0f);
	m_playerHPBarSpriteRender.SetPosition({-900.0f, 380.0f, 0.0f});
	m_playerHPBarSpriteRender.Update();

	//残機の設定
	m_playerLeftSpriteRender.Init("Assets/sprite/playerLeft.dds", 128.0f, 100.0f);
	m_playerLeftSpriteRender.SetPosition(Vector3(825.0f, 470.0f, 0.0f));
	m_playerLeftSpriteRender.Update();

	//Y缶の設定
	m_playerLeftUpSpriteRender.Init("Assets/sprite/leftUp.dds", 64.0f, 64.0f);
	m_playerLeftUpSpriteRender.SetPosition(Vector3(830.0f, 370.0f, 0.0f));
	m_playerLeftUpSpriteRender.Update();

	//武器画像の縁の設定
	m_playerWeaponFrameSpriteRender.Init("Assets/sprite/weapon_Frame.dds", 64.0f, 64.0f);
	m_playerWeaponFrameSpriteRender.SetPosition(Vector3(830.0f, 290.0f, 0.0f));
	m_playerWeaponFrameSpriteRender.Update();

	//フェードの処理
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(18, "Assets/sound/stage1_bgm.wav");

	//bgmの設定
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(18);
	m_bgm->SetVolume(gameManager->m_bgm);
	m_bgm->Play(true);

	return true;
}

void Game::Update()
{
	//g_renderingEngine->DisableRaytracing();

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!fade->IsFade()) {
			//自身を削除する
			DeleteGO(this);
		}
	}
}

void Game::IsGameOver(int playerLeft)
{
	if (!m_isWaitFadeOut) {

		if (playerLeft <= 0) {
			m_isGameOver = true;
		}

		DeleteGO(m_bgm);

		m_isWaitFadeOut = true;
		fade->StartFadeOut();
	}
}

void Game::IsGameClear()
{
	if (!m_isWaitFadeOut) {

		m_isClear = true;
		DeleteGO(m_bgm);
		m_isWaitFadeOut = true;
		fade->StartFadeOut();
	}
}

void Game::Render(RenderContext& rc)
{
	m_playerHPBarSpriteRender.Draw(rc);
	m_playerLeftSpriteRender.Draw(rc);
	m_playerLeftUpSpriteRender.Draw(rc);
	m_playerWeaponFrameSpriteRender.Draw(rc);
}
