#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{

}

bool Actor::Start()
{
	return true;
}

void Actor::Update()
{

}

void Actor::Blink()
{
	//無敵状態なら
	if (m_ivTime > 0.0f) {
		int milliSecond = (m_ivTime - (int)m_ivTime) * 10.0f;

		//明滅する
		if (milliSecond % 4 == 0) {
			m_enabled = true;
		}
		else {
			m_enabled = false;
		}
		m_ivTime -= g_gameTime->GetFrameDeltaTime();
	}
}
