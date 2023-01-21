#include "stdafx.h"
#include "GameClear.h"

#include "Fade.h"
#include "Title.h"


GameClear::GameClear()
{

}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	g_camera3D->SetPosition({ 0.0f, 50.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetNear(0.5f);
	g_camera3D->Update();

	//�摜�̐ݒ�
	m_backGroundSpriteRender.Init("Assets/sprite/png/title/backGround.dds", 1920.0f, 1080.0f);
	m_clearSpriteRender.Init("Assets/sprite/png/title/gameClear.dds", 1920.0f, 1080.0f);

	//�t�F�[�h�̏���
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	return true;
}

void GameClear::Update()
{
	//�t�F�[�h�̑ҋ@����
	if (m_isWaitFadeOut) {
		//�t�F�[�h���I������
		if (!fade->IsFade()) {
			//���j���[��ʂ֑J��
			Title* title = NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeOut = true;
			fade->StartFadeOut();
		}
	}
}

void GameClear::Render(RenderContext& rc)
{
	m_backGroundSpriteRender.Draw(rc);
	m_clearSpriteRender.Draw(rc);
}