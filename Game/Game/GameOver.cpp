#include "stdafx.h"
#include "GameOver.h"

#include "Fade.h"
#include "Game.h"
#include "StageSelect.h"
#include "Title.h"
#include "GameManager.h"


GameOver::GameOver()
{

}

GameOver::~GameOver()
{
	switch (m_cursor) {
	case 0:
		NewGO<Game>(0, "game");
		break;

	case 1:
		NewGO<Title>(0, "title");
		//NewGO<StageSelect>(0, "stageSelect");
		break;

	case 2:
		NewGO<Title>(0, "title");
		break;
	}
}

bool GameOver::Start()
{
	GameManager* gameManager = FindGO<GameManager>("gameManager");
	gameManager->ResetPlayerLeft();

	g_camera3D->SetPosition({ 0.0f, 50.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetNear(0.5f);
	g_camera3D->Update();

	//背景の設定
	m_backModelRender.Init("Assets/modelData/backGround2.tkm");
	m_backModelRender.SetPosition(Vector3(0.0f, -100.0f, 1000.0f));
	m_backModelRender.Update();

	//ゲームオーバーの設定
	m_gameOverSpriteRender.Init("Assets/sprite/png/title/gameOver.dds", 460.0f, 106.0f);

	//カーソルの設定
	m_cursorModelRender.Init("Assets/modelData/cursor.tkm");
	m_cursorModelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_cursorModelRender.SetScale(Vector3(0.4f, 0.4f, 0.4f));

	//テキストの設定
	m_fontRender.SetText(L"Retry\n\nTitle");
	//m_fontRender.SetText(L"Retry\n\nStage Select\n\nTitle");
	m_fontRender.SetPosition(Vector3(-150.0f, -200.0f, 0.0f));
	m_fontRender.SetShadowParam(true, 5.0f, Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	return true;
}

void GameOver::Update()
{
	//待機時間なら
	if (m_wait) {

		if (m_time > 2.0f)
			m_wait = false;

		m_time += g_gameTime->GetFrameDeltaTime();

		return;
	}

	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!fade->IsFade()) {
			DeleteGO(this);
		}
	}
	else {
		Input();
	}

	//カーソル設定
	m_cursorModelRender.SetPosition(Vector3(-45.0f, (-m_cursor * 20.0f) + 2.5f, 0.0f));
	m_rotation.AddRotationX(-0.05f);
	m_cursorModelRender.SetRotation(m_rotation);
	m_cursorModelRender.Update();
}

void GameOver::Input()
{
	//カーソルの移動処理
	if (g_pad[0]->IsTrigger(enButtonDown)) {
		if (m_cursor < 1)
			m_cursor++;
	}
	else if (g_pad[0]->IsTrigger(enButtonUp)) {
		if (m_cursor > 0)
			m_cursor--;
	}

	//決定処理
	if (g_pad[0]->IsTrigger(enButtonA)) {

		if (!m_isWaitFadeOut) {
			m_isWaitFadeOut = true;
			fade->StartFadeOut();
		}
	}
}

void GameOver::Render(RenderContext& rc)
{
	m_backModelRender.Draw(rc);

	m_gameOverSpriteRender.Draw(rc);

	if (!m_wait) {
		m_fontRender.Draw(rc);

		m_cursorModelRender.Draw(rc);
	}
}