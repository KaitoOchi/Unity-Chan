#pragma once


class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
};

