#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "GameCamera.h"
#include "Stage.h"
#include "Fade.h"

#include "GameManager.h"


Game::Game()
{
	//�t���[�����[�g���Œ�
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);

	//�����V�[�h�̏�����
	srand(time(NULL));

	//�����蔻���L����
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
	//�f�[�^�̏�������
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
	//�Z�[�u����
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

	//�Q�[���V�[���J�ڎ��ɕK�v�ȃN���X��NewGO
	NewGO<GameCamera>(0, "gameCamera");

	Stage* stage = NewGO<Stage>(0, "stage");
	stage->SetStageNum(gameManager->GetStageNum());
	stage->SetCourceNum(gameManager->GetCourceNum());

	Player* player = NewGO<Player>(1, "player");
	player->Actor::SetPosition(gameManager->GetContinuePosition());
	player->Actor::SetSoundVolume(gameManager->m_sfx);


	//HP�o�[�̑���
	m_playerHPBarSpriteRender.Init("Assets/sprite/hpBar.dds", 32.0f, 256.0f);
	m_playerHPBarSpriteRender.SetPosition({-900.0f, 380.0f, 0.0f});
	m_playerHPBarSpriteRender.Update();

	//�c�@�̐ݒ�
	m_playerLeftSpriteRender.Init("Assets/sprite/playerLeft.dds", 128.0f, 100.0f);
	m_playerLeftSpriteRender.SetPosition(Vector3(825.0f, 470.0f, 0.0f));
	m_playerLeftSpriteRender.Update();

	//Y�ʂ̐ݒ�
	m_playerLeftUpSpriteRender.Init("Assets/sprite/leftUp.dds", 64.0f, 64.0f);
	m_playerLeftUpSpriteRender.SetPosition(Vector3(830.0f, 370.0f, 0.0f));
	m_playerLeftUpSpriteRender.Update();

	//����摜�̉��̐ݒ�
	m_playerWeaponFrameSpriteRender.Init("Assets/sprite/weapon_Frame.dds", 64.0f, 64.0f);
	m_playerWeaponFrameSpriteRender.SetPosition(Vector3(830.0f, 290.0f, 0.0f));
	m_playerWeaponFrameSpriteRender.Update();

	//�t�F�[�h�̏���
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(18, "Assets/sound/stage1_bgm.wav");

	//bgm�̐ݒ�
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(18);
	m_bgm->SetVolume(gameManager->m_bgm);
	m_bgm->Play(true);

	return true;
}

void Game::Update()
{
	//g_renderingEngine->DisableRaytracing();

	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!fade->IsFade()) {
			//���g���폜����
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
