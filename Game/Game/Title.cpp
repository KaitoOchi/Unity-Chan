#include "stdafx.h"
#include "Title.h"

#include "Character.h"
#include "Fade.h"
#include "Menu.h"
#include "GameManager.h"


Title::Title()
{
	//�f�B���N�V���i�����C�g
	g_renderingEngine->SetDirectionLight(
		0,					// ���C�g�̔ԍ�
		{ 1, -1, 0.75 },			// ����
		{ 0, 0, 0 }		// �J���[
	);
}

Title::~Title()
{
	DeleteGO(character);
	DeleteGO(m_bgm);
}

bool Title::Start()
{	
	//�A���r�G���g���C�g
	g_renderingEngine->SetAmbient({ 0.5f, 0.5f, 0.5f });
	//�g�[���}�b�v�A�f�t�H���g��0.18
	g_renderingEngine->SetSceneMiddleGray(0.48f);

	//�摜�̐ݒ�
	m_rogoSpriteRender.Init("Assets/sprite/png/title/title.dds", 1920.0f, 1080.0f);
	m_pressSpriteRender.Init("Assets/sprite/png/title/press_A_Button.dds", 1920.0f, 1080.0f);

	m_backGroundModelRender.Init("Assets/modelData/backGround.tkm", nullptr, 0, enModelUpAxisZ, false);
	m_backGroundModelRender.SetPosition(Vector3(0.0f, -100.0f, 75.0f));
	m_backGroundModelRender.Update();

	//�L�����N�^�[�̔z�u
	character = NewGO<Character>(0, "character");
	character->SetPosition(Vector3(-100.0f, -50.0f, 50.0f));
	character->SetRotation(150.0f);
	character->SetCanMove(false);

	//�t�F�[�h�̏���
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/bgm_title.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/enter.wav");

	gameManager = FindGO<GameManager>("gameManager");

	//bgm�̐ݒ�
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(11);
	m_bgm->SetVolume(gameManager->m_bgm);
	m_bgm->Play(true);

	return true;
}

void Title::Update()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!fade->IsFade()) {
			//���j���[��ʂ֑J��
			Menu* menu = NewGO<Menu>(0, "menu");
			DeleteGO(this);
		}
	}
	else {
		//����{�^��
		if (g_pad[0]->IsTrigger(enButtonA)) {

			if (m_cursor == 12) {
				g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");
				g_soundEngine->ResistWaveFileBank(1, "Assets/sound/weapon01.wav");
				g_soundEngine->ResistWaveFileBank(5, "Assets/sound/damage.wav");
				g_soundEngine->ResistWaveFileBank(6, "Assets/sound/dead.wav");
				g_soundEngine->ResistWaveFileBank(9, "Assets/sound/heal.wav");
				g_soundEngine->ResistWaveFileBank(10, "Assets/sound/appear.wav");
				g_soundEngine->ResistWaveFileBank(11, "Assets/sound/bgm_title.wav");
				g_soundEngine->ResistWaveFileBank(12, "Assets/sound/enter.wav");
				g_soundEngine->ResistWaveFileBank(13, "Assets/sound/select.wav");

				m_isSoundTest = true;
				m_cursor = 0;
				m_bgm->Stop();
				return;
			}

			m_isWaitFadeOut = true;
			fade->StartFadeOut();

			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(12);
			se->SetVolume(gameManager->m_sfx);
			se->Play(false);
		}

		if (g_pad[0]->IsTrigger(enButtonB)) {
			m_cursor++;
			m_isSoundTest = false;
		}
	}

	if (m_isSoundTest)
		SoundTest();

	//���Ԃ̏���
	m_time += g_gameTime->GetFrameDeltaTime();
	if (m_time > 1.0f)
		m_time = -1.0f;

	// (-2 * t^3) + (3 * t^2)
	float alpha = fabsf((-2 * pow(m_time, 3.0f)) + (3 * pow(m_time, 2.0f)));
	//�����x��ύX
	m_pressSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
	m_pressSpriteRender.Update();
}

void Title::SoundTest()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//�T�E���h���Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(m_cursor);
		se->SetVolume(gameManager->m_sfx);
		se->Play(false);
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		if (m_cursor > 0)
			m_cursor--;
	}
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		if (m_cursor < 13)
			m_cursor++;
	}

	//�e�L�X�g�̐ݒ�
	wchar_t text[255];
	swprintf_s(text, 255, L"Sound Test \nNumber:%d", m_cursor);
	m_soundTestFontRender.SetText(text);
}

void Title::Render(RenderContext& rc)
{
	m_backGroundModelRender.Draw(rc);
	m_rogoSpriteRender.Draw(rc);

	if (m_isSoundTest) {
		m_soundTestFontRender.Draw(rc);
		return;
	}

	m_pressSpriteRender.Draw(rc);
}