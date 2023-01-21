#include "stdafx.h"
#include "VanishFloorManager.h"

#include "Floor.h"
#include "FloorRender.h"
#include "GameManager.h"


VanishFloorManager::VanishFloorManager()
{

}

VanishFloorManager::~VanishFloorManager()
{

}

bool VanishFloorManager::Start()
{
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/vanish_floor.wav");

	GameManager* gameManager = FindGO<GameManager>("gameManager");
	m_volume = gameManager->m_sfx;

	return true;
}

void VanishFloorManager::Update()
{
	m_frameCount++;

	if (m_frameCount % 75 == 0) {

		//最大値を超えたら
		if (m_activeNum > m_floorNum) {
			m_frameCount = 0;
			m_activeNum = 0;
		}

		//足場を増やす
		if (m_activeNum < m_floorNum) {
			floor[m_activeNum]->SetPosition(m_floorPosition[m_activeNum]);

			//出現音を出す
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(10);
			se->Play(false);
			se->SetVolume(m_volume);
		}

		//足場を減らす
		if (m_activeNum >= 2) {
			floor[m_activeNum - 2]->SetPosition(Vector3(m_floorPosition[m_activeNum - 2].x, m_floorPosition[m_activeNum - 2].y + 10000.0f, m_floorPosition[m_activeNum - 2].z + 300.0f));
		}
		else if(m_activeNum == 0) {
			floor[m_floorNum - 1]->SetPosition(Vector3(m_floorPosition[m_floorNum - 1].x, m_floorPosition[m_floorNum - 1].y + 10000.0f, m_floorPosition[m_floorNum - 1].z + 300.0f));
		}

		m_activeNum++;
	}
}

void VanishFloorManager::SpawnFloor()
{
	int vanishFloorNum = 0;

	for (int i = 0; i < 10; i++) {
		
		//座標が0なら終了
		if (m_floorPosition[i].x == 0.0f && m_floorPosition[i].y == 0.0) {

			if (i == 0) {
				DeleteGO(this);
			}

			m_floorNum = i;
			break;
		}

		floor[i] = NewGO<Floor>(0, "floor");
		floor[i]->SetPosition(m_floorPosition[i]);
		floor[i]->SetType(2);
		floor[i]->SetInstanceNo(vanishFloorNum);
		floor[i]->SetPosition(Vector3(m_floorPosition[i].x, m_floorPosition[i].y + 10000.0f, m_floorPosition[i].z + 300.0f));

		vanishFloorNum++;
	}

	if (vanishFloorNum != 0) {
		//インスタンシング描画の設定
		FloorRender* vanishFloorRender = NewGO<FloorRender>(0, "floorRender");
		vanishFloorRender->SetMaxSphere(vanishFloorNum);
		vanishFloorRender->SetType(1);
	}
}