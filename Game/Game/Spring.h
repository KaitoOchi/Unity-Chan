#pragma once
#include "Object.h"


class Spring : public IGameObject, public Object
{
public:
	Spring();
	~Spring();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

private:
};

