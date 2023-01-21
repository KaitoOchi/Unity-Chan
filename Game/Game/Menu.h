#pragma once

class Fade;
class GameManager;

class Menu : public IGameObject
{
public:
	Menu();
	~Menu();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	//���͏���
	void Input();
	//�������̏���
	void Text();
	//�J�[�\���ړ��̏���
	void Select(const int select);
	//���菈��
	void StateTransition(const bool num);
	//�X�e�[�W�Z���N�g�V�[���̑J�ڏ���
	void NotifyStageSelect(bool isStageClear[], bool getItem[], const int leftUp, const bool isAll);

	ModelRender			m_backGroundModelRender;
	ModelRender			m_cursorModelRender;
	SpriteRender		m_menuSpriteRender;
	SpriteRender		m_settingSpriteRender;
	SpriteRender		m_explainSpriteRender;
	SpriteRender		m_bgmSpriteRender[10];
	SpriteRender		m_sfxSpriteRender[10];
	SpriteRender		m_difficultSpriteRender[2];
	FontRender			m_debugFontRender;
	FontRender			m_explainFontRender;

	Fade*				fade = nullptr;
	GameManager*		gameManager = nullptr;

	Quaternion			m_rotation;

	const wchar_t* m_message = nullptr;

	const wchar_t m_message_menu[6][255] = {L"�������傩�� �͂��߂�",
											 L"������@�� �m�F����",
											 L"�ݒ�� �ύX����",
											 L"BGM�� �ύX���܂�",
										 	 L"���ʉ��� �ύX���܂�",
											 L"�Q�[�����[�h�� �ύX���܂�",
											};

	char				font[5][5];
	bool				password[5][5] = { { 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 },
											{ 0,0,0,0,0 } };

	bool				m_isWaitFadeOut = false;

	int					m_state = 0;		//�X�e�[�g
	int					m_hCursor = 0;		//���������̃J�[�\��
	int					m_vCursor = 0;		//���������̃J�[�\��
	int					m_maxCursor = 2;	//�J�[�\���̍ő�l
	int					m_maxHCursor[3] = { 10, 10, 1 };
	int					m_settingNum[3] = { 5, 5, 0 };

	//�Z�[�u�f�[�^
	//0�`3�@�N���A�����X�e�[�W
	//4�`7�@�A�C�e���擾�����X�e�[�W
	//8		Y�ʂ̗�
	//9		�Q�[�����[�h
	//10	bgm
	//11	sfx
	int m_saveData[12] = { 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 2 };
};