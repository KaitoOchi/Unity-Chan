#include "stdafx.h"
#include "StageSelect.h"

#include "Fade.h"
#include "GameManager.h"
#include "Character.h"


StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{
	DeleteGO(character);
}

bool StageSelect::Start()
{
	gameManager = FindGO<GameManager>("gameManager");

	//�L�����N�^�[�̔z�u
	character = NewGO<Character>(0, "character");
	character->SetPosition(Vector3(-300.0f, -400.0f, 700.0f));
	character->SetRotation(90.0f);
	character->SetCanMove(true);

	//�����̐ݒ�
	wchar_t text[255];
	swprintf_s(text, 255, L"Ykan:%d \nIs:%d \nIsStageClear[]:%d%d%d%d",
				gameManager->m_leftUp, gameManager->m_isOwata, gameManager->m_isStageClear[0], gameManager->m_isStageClear[1], gameManager->m_isStageClear[2], gameManager->m_isStageClear[3]);
	m_fontRender.SetPosition(Vector3(0.0f, 300.0f, 0.0f));
	m_fontRender.SetText(text);

	//�t�F�[�h�̏���
	fade = FindGO<Fade>("fade");
	fade->StartFadeIn();

	return true;
}

void StageSelect::Update()
{

}

void StageSelect::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}