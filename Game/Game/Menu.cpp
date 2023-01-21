#include "stdafx.h"
#include "Menu.h"

#include "Fade.h"
#include "GameManager.h"
#include "StageSelect.h"
#include "Game.h"


Menu::Menu()
{
	//�o�C�i���t�@�C�����������������ꍇ�A�R�����g�A�E�g
	/*
	FILE* fp2 = fopen("saveData.bin", "wb");
	fwrite(m_saveData, sizeof(m_saveData), 1, fp2);
	fclose(fp2);
	*/
}

Menu::~Menu()
{
	/*
	m_saveData[10] = m_settingNum[0];
	m_saveData[11] = m_settingNum[1];

	//�Z�[�u����
	FILE* fp = fopen("saveData.bin", "wb");
	fwrite(m_saveData, sizeof(m_saveData), 1, fp);
	fclose(fp);
	*/
}

bool Menu::Start()
{
	/*
	//�o�C�i���t�@�C���̃��[�h
	FILE* fp = fopen("saveData.bin", "rb");
	if (fp != NULL) {
		fread(m_saveData, sizeof(m_saveData), 1, fp);
		fclose(fp);
	}
	*/

	gameManager = FindGO<GameManager>("gameManager");
	//gameManager->m_bgm = (float)m_saveData[10] / 10.0f;
	//gameManager->m_sfx = (float)m_saveData[11] / 10.0f;
	m_settingNum[0] = gameManager->m_bgm * 10;
	m_settingNum[1] = gameManager->m_sfx * 10;

	//�w�i�̐ݒ�
	m_backGroundModelRender.Init("Assets/modelData/backGround.tkm", nullptr, 0, enModelUpAxisZ, false);
	m_backGroundModelRender.SetPosition(Vector3(0.0f, -100.0f, 75.0f));
	m_backGroundModelRender.Update();

	//�J�[�\���̐ݒ�
	m_cursorModelRender.Init("Assets/modelData/cursor.tkm");

	//���j���[���
	m_menuSpriteRender.Init("Assets/sprite/png/title/Menu.dds", 1920.0f, 1080.0f);

	//�ݒ���
	m_settingSpriteRender.Init("Assets/sprite/png/title/setting.dds", 1920.0f, 1080.0f);

	//�������
	m_explainSpriteRender.Init("Assets/sprite/explain.dds", 1920.0f, 1080.0f);

	//bgm��sfx�摜�̐ݒ�
	for (int i = 0; i < 10; i++) {
		m_bgmSpriteRender[i].Init("Assets/sprite/png/title/bar.dds", 127.0f, 88.0f);
		m_bgmSpriteRender[i].SetPosition(Vector3((i * 60.0f) - 125.0f, 190.0f, 0.0f));
		m_bgmSpriteRender[i].Update();

		m_sfxSpriteRender[i].Init("Assets/sprite/png/title/bar.dds", 127.0f, 88.0f);
		m_sfxSpriteRender[i].SetPosition(Vector3((i * 60.0f) - 125.0f, -0.0f, 0.0f));
		m_sfxSpriteRender[i].Update();
	}

	//�����̐ݒ�
	m_explainFontRender.SetPosition(Vector3(-200.0f, -400.0f, 0.0f));
	m_explainFontRender.SetPivot(Vector2(1.0f, 0.5f));

	//��Փx�摜�̐ݒ�
	m_difficultSpriteRender[0].Init("Assets/sprite/png/title/normal.dds", 256.0f, 128.0f);
	m_difficultSpriteRender[0].SetPosition(Vector3(125.0f, -200.0f, 0.0f));
	m_difficultSpriteRender[0].Update();
	m_difficultSpriteRender[1].Init("Assets/sprite/png/title/owata.dds", 256.0f, 128.0f);
	m_difficultSpriteRender[1].SetPosition(Vector3(125.0f, -200.0f, 0.0f));
	m_difficultSpriteRender[1].Update();

	//�t�F�[�h�̏���
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/enter.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/select.wav");

	m_debugFontRender.SetPosition(Vector3(500.0f, 0.0f, 0.0f));

	Text();

	return true;
}

void Menu::Update()
{
	//�J�[�\���ݒ�
	m_cursorModelRender.SetPosition(Vector3((m_state * -30.0f) + -100.0f, (-m_vCursor * 45.0f) + 90.0f, 0.0f));
	m_rotation.AddRotationX(-0.05f);
	m_cursorModelRender.SetRotation(m_rotation);
	m_cursorModelRender.Update();

	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!fade->IsFade()) {
			//�X�e�[�W�Z���N�g�֑J��
			//NewGO<StageSelect>(0, "stageSelect");
			NewGO<Game>(0, "game");
			DeleteGO(this);
		}
		return;
	}

	Input();

	wchar_t debugText[255];
	swprintf_s(debugText, 255, L"H:%d \nV:%d \nstate:%d \nMax:%d \nBGM:%.2f \nSFX:%.2f", m_hCursor, m_vCursor, m_state, m_maxCursor, gameManager->m_bgm, gameManager->m_sfx);
	m_debugFontRender.SetText(debugText);
}

void Menu::Input()
{
	//�J�[�\���̈ړ�����
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		if (m_vCursor > 0)
			Select(1);

		if (m_state == 2)
			m_hCursor = m_settingNum[m_vCursor];
	}
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		if (m_vCursor < m_maxCursor)
			Select(3);

		if (m_state == 2)
			m_hCursor = m_settingNum[m_vCursor];
	}

	if (m_state == 2) {
		//�J�[�\���̈ړ�����
		if (g_pad[0]->IsTrigger(enButtonLeft)) {
			if (m_hCursor > 0) {
				Select(4);
			}

		}
		else if (g_pad[0]->IsTrigger(enButtonRight)) {
			if (m_hCursor < m_maxHCursor[m_vCursor]) {
				Select(2);
			}
		}
	}

	//���菈��
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if(m_vCursor < 3)
			StateTransition(true);
	}
	//�L�����Z��
	else if (g_pad[0]->IsTrigger(enButtonB)) {
		if (m_state > 0)
			StateTransition(false);
	}
}

void Menu::Text()
{
	int messageNum = 0;

	if (m_state == 2)
		messageNum = 3;

	m_message = m_message_menu[m_vCursor + messageNum];

	wchar_t text[255];
	swprintf_s(text, 255, L"%s", m_message);
	m_explainFontRender.SetText(text);
}

void Menu::Select(const int select)
{
	switch (select) {
	case 1:
		m_vCursor--;
		break;
	case 2:
		m_hCursor++;
		break;
	case 3:
		m_vCursor++;
		break;
	case 4:
		m_hCursor--;
		break;
	}

	//�ݒ�Ȃ�
	if (m_state == 2 && (select % 2 == 0)) {

		switch (m_vCursor) {
		case 0:
			gameManager->m_bgm = (float)m_hCursor / 10.0f;
			break;
		case 1:
			gameManager->m_sfx = (float)m_hCursor / 10.0f;
			break;
		}

		m_settingNum[m_vCursor] = m_hCursor;
	}

	if (m_state != 3) {
		//�Z���N�g���̐ݒ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(13);
		se->SetVolume(gameManager->m_sfx);
		se->Play(false);
	}

	Text();
}

void Menu::StateTransition(const bool state)
{
	//����{�^���������ꂽ��
	if (state) {

		if (m_state != 0)
			return;

		bool isStageClear[4] = { 0, 0, 0, 0 };
		bool getItem[4] = { 0, 0, 0, 0 };
		int leftUp = 0;
		bool owata = false;

		switch (m_vCursor) {
		//���߂����I��������
		case 0:
			NotifyStageSelect(isStageClear, getItem, leftUp, m_settingNum[2]);
			break;

		//���������I��������
		case 1:

			m_state = 3;
			/*
			//�f�[�^�̓ǂݍ���
			for (int i = 0; i < 4; i++) {
				isStageClear[i] = m_saveData[i];
				getItem[i] = m_saveData[i + 4];
			}
			leftUp = m_saveData[8];
			owata = m_saveData[9];

			NotifyStageSelect(isStageClear, getItem, leftUp, owata);
			*/
			break;

		//�ݒ��I��������
		case 2:
			m_state = 2;
			m_maxCursor = 2;
			m_hCursor = m_settingNum[0];
			break;
		}

		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(12);
		se->SetVolume(gameManager->m_sfx);
		se->Play(false);
	}
	//�L�����Z���{�^���������ꂽ��
	else {
		m_state = 0;
		m_maxCursor = 3;
		m_hCursor = 0;
	}

	//�J�[�\���ʒu�̃��Z�b�g
	m_vCursor = 0;

	Text();
}

void Menu::NotifyStageSelect(bool isStageClear[], bool getItem[], const int leftUp, const bool owata)
{
	//�X�e�[�^�X�̐ݒ�
	for (int i = 0; i < 4; i++) {
		gameManager->m_isStageClear[i] = isStageClear[i];
		gameManager->m_getItem[i] = getItem[i];
	}
	gameManager->m_leftUp = leftUp;
	gameManager->m_isOwata = owata;

	if (!m_isWaitFadeOut) {
		m_isWaitFadeOut = true;
		fade->StartFadeOut();
	}
}

void Menu::Render(RenderContext& rc)
{
	//�w�i�̕`��
	m_backGroundModelRender.Draw(rc);

	//���j���[�̕`��
	if (m_state == 0) {
		m_menuSpriteRender.Draw(rc);
	}

	if (m_state == 3) {
		m_explainSpriteRender.Draw(rc);
	}
	else {
		m_cursorModelRender.Draw(rc);

		m_explainFontRender.Draw(rc);
	}

	if (m_state == 2) {
		//�ݒ�̕`��
		m_settingSpriteRender.Draw(rc);

		//bgm�̕`��
		for (int i = 0; i < gameManager->m_bgm * 10; i++) {
			m_bgmSpriteRender[i].Draw(rc);
		}
		//sfx�̕`��
		for (int i = 0; i < gameManager->m_sfx * 10; i++) {
			m_sfxSpriteRender[i].Draw(rc);
		}
		//�Q�[�����[�h�̕`��
		if (m_settingNum[2] == 0) {
			m_difficultSpriteRender[0].Draw(rc);
		}
		else if (m_settingNum[2] == 1) {
			m_difficultSpriteRender[1].Draw(rc);
		}
	}

	//m_debugFontRender.Draw(rc);
}

/* �p�X���[�h�V�X�e���͍폜���܂����B���{�B

  { { 3,4,3,1,1 },
	{ 3,3,4,3,3 },
	{ 1,3,1,3,4 },
	{ 1,3,3,4,3 },
	{ 2,2,2,2,2 } };

	1..Y�ʂ̐�
	2..�`�F�b�N�T��
	3..�X�e�[�W�N���A���
	4..�A�C�e���擾���


void Menu::Draw()
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {

			if (password[i][j] == true) {
				font[i][j] = 'X';
			}
			else {
				font[i][j] = ' ';
			}
		}
	}

	wchar_t text[255];
	swprintf_s(text, 255, L"%c %c %c %c %c \n%c %c %c %c %c \n%c %c %c %c %c \n%c %c %c %c %c \n%c %c %c %c %c",
		font[0][0], font[0][1], font[0][2], font[0][3], font[0][4],
		font[1][0], font[1][1], font[1][2], font[1][3], font[1][4],
		font[2][0], font[2][1], font[2][2], font[2][3], font[2][4],
		font[3][0], font[3][1], font[3][2], font[3][3], font[3][4],
		font[4][0], font[4][1], font[4][2], font[4][3], font[4][4]);
	m_fontRender.SetText(text);
}

void Menu::EnterPassWord()
{
	int xNum[5] = { 0, 0, 0, 0, 0 };		//16�i���̐��l
	int checkSum = 0;						//�`�F�b�N�T��
	float powNum = 0;						//�ݏ�

	bool isAll = true;						//���ׂĖ��܂��Ă��邩�ǂ���
	bool isStageClear[4] = { 0, 0, 0, 0 };	//�X�e�[�W�N���A���
	bool getItem[4] = { 0, 0, 0, 0 };		//�A�C�e���̎擾���
	int leftUp = 0;							//Y�ʂ̏��������

	for (int i = 0; i < 5; i++) {
		powNum = 0.0f;
		for (int j = 0; j < 5; j++) {

			//��ł����܂��ĂȂ��Ȃ�
			if (password[i][j] == false)
				isAll = false;

			//2�i����10�i���ɕϊ�
			if (password[i][j] == true)
				xNum[i] += pow(2, powNum);

			powNum += 1.0f;
		}
	}

	//���ׂĖ��܂��Ă���Ȃ�
	if (isAll) {
		NotifyStageSelect(isStageClear, getItem, leftUp, isAll);
		return;
	}

	//�`�F�b�N�T��
	checkSum = xNum[0] + xNum[1] + xNum[2] + xNum[3];
	//��̈ʂƗv�f4����v���Ȃ��Ȃ�
	if (checkSum - (checkSum / 10 * 10) != xNum[4]) {

		xNum[0] = xNum[0];
		return;
	}

	xNum[0] = xNum[0];

	//Y�ʂ̐���ݒ�
	if (password[2][2])
		leftUp += 1;
	if (password[0][3])
		leftUp += 2;
	if (password[3][0])
		leftUp += 4;
	if (password[2][0])
		leftUp += 8;
	if (password[0][4])
		leftUp = 9;

	//�X�e�[�W�N���A����ݒ�
	if (password[1][1] && password[1][3] && password[3][1])
		isStageClear[0] = true;
	if (password[2][1] && password[0][2] && password[2][3])
		isStageClear[1] = true;
	if (password[0][0] && password[3][4] && password[3][2])
		isStageClear[2] = true;
	if (password[1][0] && password[1][4])
		isStageClear[3] = true;

	//�A�C�e���擾���
	if (password[1][2])
		getItem[0] = true;
	if (password[3][3])
		getItem[1] = true;
	if (password[0][1])
		getItem[0] = true;
	if (password[2][4])
		getItem[0] = true;

	NotifyStageSelect(isStageClear, getItem, leftUp, isAll);
}
*/