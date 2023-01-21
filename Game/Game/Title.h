#pragma once


class Character;
class Fade;
class GameManager;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void SoundTest();

	ModelRender			m_backGroundModelRender;
	SpriteRender		m_rogoSpriteRender;
	SpriteRender		m_pressSpriteRender;
	FontRender			m_soundTestFontRender;

	SoundSource*		m_bgm = nullptr;
	Character*			character = nullptr;
	Fade*				fade = nullptr;
	GameManager*		gameManager = nullptr;

	bool				m_isWaitFadeOut = false;	//�t�F�[�h�����ǂ���
	bool				m_isSoundTest = false;		//�T�E���h�e�X�g�����ǂ���
	int					m_cursor = 0;				//�J�[�\��
	float				m_time = 0.0f;				//����
};

