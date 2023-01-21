#include "stdafx.h"
#include "Fade.h"


Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//�Ó]���̉摜
	m_spriteRender.Init("Assets/sprite/black.dds", 1920.0f, 1080.0f);
	return true;
}

void Fade::Update()
{
	switch (state)
	{
		//�t�F�[�h�C�����Ȃ�
	case enState_FadeIn:
		m_alpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			state = enState_Idle;
		}
		break;

		//�t�F�[�h�A�E�g���Ȃ�
	case enState_FadeOut:
		m_alpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			state = enState_Idle;
		}
		break;

		//�ҋ@���Ȃ�
	case enState_Idle:
		break;
	}

	m_spriteRender.Update();

}

void Fade::Render(RenderContext& rc)
{
	if (m_alpha > 0.0f)
	{
		m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_alpha });
		m_spriteRender.Draw(rc);
	}
}