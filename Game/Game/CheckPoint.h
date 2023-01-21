#pragma once
#include "Object.h"


class CheckPoint : public IGameObject, public Object
{
public:
	CheckPoint();
	~CheckPoint();
	bool Start() override;
	void Update() override;

private:
	int					m_checkNum = 0;		//チャックポイントの番号
};

