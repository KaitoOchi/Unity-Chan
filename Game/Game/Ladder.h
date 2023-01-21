#pragma once
#include "Object.h"


class Ladder : public IGameObject, public Object
{
public:
	Ladder();
	~Ladder();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

	//������ݒ肷��
	inline void SetHeight(float h) { m_height = h; }

private:
	float				m_height = 0.0f;		//����
};

