#include "stdafx.h"
#include "Ykan.h"

#include "Player.h"
#include "GameManager.h"


Ykan::Ykan()
{

}

Ykan::~Ykan()
{

}

bool Ykan::Start()
{
	player = FindGO<Player>("player");
	gameManager = FindGO<GameManager>("gameManager");

	//���f���̐ݒ�
	m_modelRender.SetRaytracingWorld(false);
	m_modelRender.Init("Assets/modelData/object/Ykan.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/getItem.wav");

	return true;
}

void Ykan::Update()
{
	//�v���C���[����Y�ʂɌ������x�N�g�����v�Z�B
	Vector3 diff = player->GetPosition() - m_position;
	//�x�N�g���̒�����������������B
	if (diff.Length() <= 60.0f) {

		//Y�ʂ𑝂₷
		gameManager->m_leftUp += 1;
		gameManager->SetGetYkan(true);
		player->Font();

		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(23);
		se->SetVolume(gameManager->m_sfx);
		se->Play(false);

		DeleteGO(this);
	}
}

void Ykan::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
