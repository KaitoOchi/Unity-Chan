#pragma once

class Fade;

class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_clearSpriteRender;
	SpriteRender m_backGroundSpriteRender;

	Fade* fade = nullptr;

	int stageNum = 0;

	bool m_isWaitFadeOut = false;
};