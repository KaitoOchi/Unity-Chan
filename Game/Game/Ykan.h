#pragma once


class Player;
class GameManager;

class Ykan : public IGameObject
{
public:
	Ykan();
	~Ykan();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//À•W‚ğİ’è
	inline void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	ModelRender m_modelRender;

	Player* player = nullptr;
	GameManager* gameManager = nullptr;

	Vector3 m_position;
};

