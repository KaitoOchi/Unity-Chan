#pragma once


class Fade;

class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	void Input();

	ModelRender			m_backModelRender;
	ModelRender			m_cursorModelRender;
	SpriteRender		m_gameOverSpriteRender;
	FontRender			m_fontRender;
	Fade*				fade = nullptr;
	Quaternion			m_rotation;

	bool				m_isWaitFadeOut = false;
	bool				m_wait = true;				//ウェイト
	int					m_cursor = 0;				//カーソル
	float				m_time = 0;					//時間
};