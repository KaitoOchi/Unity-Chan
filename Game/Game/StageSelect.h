#pragma once


class Fade;
class GameManager;
class Character;

class StageSelect : public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	FontRender m_fontRender;

	Fade* fade = nullptr;
	GameManager* gameManager = nullptr;
	Character* character = nullptr;
};

