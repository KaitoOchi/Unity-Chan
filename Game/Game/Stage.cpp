#include "stdafx.h"
#include "Stage.h"

#include "Game.h"
#include "Fade.h"
#include "GameOver.h"
#include "GameClear.h"
#include "GameManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Ladder.h"
#include "Spring.h"
#include "Floor.h"
#include "VanishFloorManager.h"
#include "Spike.h"
#include "CheckPoint.h"
#include "Box.h"
#include "Ykan.h"
#include "Wall.h"
#include "Outside.h"
#include "InvicibleWall.h"

#include "FloorRender.h"

#include "nature/SkyCube.h"
#include "level3DRender/LevelRender.h"

Stage::Stage()
{

}

Stage::~Stage()
{
	DeleteGO(m_skyCube);
}

bool Stage::Start()
{
	gameManager = FindGO<GameManager>("gameManager");

	//�X�e�[�W���쐬
	m_modelRender.Init(m_stageName[m_stageNum]);
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_backGroundModelRender.SetRaytracingWorld(false);
	m_backGroundModelRender.Init(m_backGroundNum[m_stageNum]);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(10.0f);

	//�f�B���N�V���i�����C�g
	g_renderingEngine->SetDirectionLight(
		0,					// ���C�g�̔ԍ�
		{ 1, -1, 0.75 },			// ����
		{ 200, 200, 200 }		// �J���[
	);
	//�A���r�G���g���C�g
	g_renderingEngine->SetAmbient({ 2, 2, 2 });
	//���C�g���[�V���O��L��
	//g_renderingEngine->EnableRaytracing();
	//�g�[���}�b�v�A�f�t�H���g��0.18
	g_renderingEngine->SetSceneMiddleGray(0.48f);
	//�u���[��
	g_renderingEngine->SetBloomThreshold(10.0f);
	//�J�X�P�[�h�V���h�E�̋߉e�G���A��
	g_renderingEngine->SetCascadeNearAreaRates(0.05f, 0.5f, 0.9f);

	// �����̌v�Z�̂��߂�IBL�e�N�X�`�����Z�b�g����B
	//g_renderingEngine->SetAmbientByIBLTexture(skyCube->GetTextureFilePath(), 0.1f);

	switch (m_stageNum) {
	case 0:
		for (int i = 0; i < 9; i++) {
			m_courceName[i] = m_cource0_storage[i];
		}
		break;

	case 1:
		for (int i = 0; i < 9; i++) {
			m_courceName[i] = m_cource1_storage[i];
		}
		m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_DayToon_3);
		break;

	case 2:
		break;

	case 3:
		break;
	}

	m_levelRender = new LevelRender;
	InitLevelRender();

	return true;
}

void Stage::DeleteObject(bool changeStage, bool isClear, bool isGameOver)
{
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		DeleteGO(enemy);
		return true;
		});

	QueryGOs<Boss>("boss", [&](Boss* boss) {
		DeleteGO(boss);
		return true;
		});

	QueryGOs<Ladder>("ladder", [&](Ladder* ladder) {
		DeleteGO(ladder);
		return true;
		});

	QueryGOs<Spring>("spring", [&](Spring* spring) {
		DeleteGO(spring);
		return true;
		});

	QueryGOs<Floor>("floor", [&](Floor* floor) {
		DeleteGO(floor);
		return true;
		});

	QueryGOs<VanishFloorManager>("vanishFloorManager", [&](VanishFloorManager* vanishFloorManager) {
		DeleteGO(vanishFloorManager);
		return true;
		});

	QueryGOs<Spike>("spike", [&](Spike* spike) {
		DeleteGO(spike);
		return true;
		});

	QueryGOs<CheckPoint>("checkPoint", [&](CheckPoint* checkPoint) {
		DeleteGO(checkPoint);
		return true;
		});

	QueryGOs<Box>("box", [&](Box* box) {
		DeleteGO(box);
		return true;
		});

	QueryGOs<Ykan>("ykan", [&](Ykan* ykan) {
		DeleteGO(ykan);
		return true;
		});

	QueryGOs<Wall>("wall", [&](Wall* wall) {
		DeleteGO(wall);
		return true;
		});

	QueryGOs<Outside>("outside", [&](Outside* outside) {
		DeleteGO(outside);
		return true;
		});

	QueryGOs<InvicibleWall>("invicibleWall", [&](InvicibleWall* invicibleWall) {
		DeleteGO(invicibleWall);
		return true;
		});

	QueryGOs<FloorRender>("floorRender", [&](FloorRender* floorRender) {
		DeleteGO(floorRender);
		return true;
		});

	//�X�e�[�W�J�ڒ��Ȃ�
	if (changeStage) {
		m_courceNum++;
		m_levelRender = new LevelRender;
		InitLevelRender();
		return;
	}

	//�N���A�������ǂ���
	if (isClear) {
		NewGO<GameClear>(3, "gameClear");
		DeleteGO(this);
	}
	//�Q�[���I�[�o�[��Ԃ��ǂ���
	else if (isGameOver) {
		NewGO<GameOver>(3, "gameover");
		DeleteGO(this);
	}
	else {
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Stage::InitLevelRender()
{
	VanishFloorManager* vanishFloorManager = NewGO<VanishFloorManager>(0, "vanishFloorManager");
	
	int moveFloorNum = 0;
	int fallFloorNum = 0;
	int conveyorNum = 0;

	//���x���f�U�C������
	m_levelRender->Init(m_courceName[m_courceNum], [&](LevelObjectData& objData) {

		//���O��enemy�Ȃ�
		if (objData.ForwardMatchName(L"enemy") == true) {
			Enemy* enemy = NewGO<Enemy>(0, "enemy");
			enemy->Actor::SetPosition(objData.position);
			enemy->SetNumber(objData.number);
			enemy->Actor::SetSoundVolume(gameManager->m_sfx);
			return true;
		}
		//���O��boss�Ȃ�
		else if (objData.ForwardMatchName(L"boss") == true) {
			Boss* boss = NewGO<Boss>(0, "boss");
			boss->Actor::SetPosition(objData.position);
			boss->SetNumber(objData.number);
			boss->Actor::SetSoundVolume(gameManager->m_sfx);
		}
		//���O��ladder�Ȃ�
		else if (objData.EqualObjectName(L"ladder") == true) {
			Ladder* ladder = NewGO<Ladder>(0, "ladder");
			ladder->Object::SetPosition(objData.position);
			ladder->SetHeight(300.0f);
			return true;
		}
		//���O��spring�Ȃ�
		else if (objData.EqualObjectName(L"spring") == true) {
			Spring* spring = NewGO<Spring>(0, "spring");
			spring->Object::SetPosition(objData.position);
			return true;
		}
		//���O��moveFloor�Ȃ�
		else if (objData.ForwardMatchName(L"moveFloor_vertical") == true) {
			Floor *floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->SetDistance(objData.number);
			floor->SetSpeed(Vector3(0.0f, 200.0f, 0.0f));
			floor->Object::SetType(0);
			floor->SetInstanceNo(moveFloorNum);
			moveFloorNum++;
			return true;
		}
		//���O��moveFloor�Ȃ�
		else if (objData.ForwardMatchName(L"moveFloor_horizontal") == true) {
			Floor* floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->SetDistance(objData.number);
			floor->SetSpeed(Vector3(200.0f, 0.0f, 0.0f));
			floor->Object::SetType(0);
			floor->SetInstanceNo(moveFloorNum);
			moveFloorNum++;
			return true;
		}
		//���O��fallFloor�Ȃ�
		else if (objData.EqualObjectName(L"fallFloor") == true) {
			Floor* floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->Object::SetType(1);
			floor->SetInstanceNo(fallFloorNum);
			fallFloorNum++;
			return true;
		}
		//���O��vanishFloor�Ȃ�
		else if (objData.ForwardMatchName(L"vanishFloor") == true) {
			vanishFloorManager->SetPosition(objData.position, objData.number);
			return true;
		}
		//���O��conveyor�Ȃ�
		else if (objData.ForwardMatchName(L"conveyor") == true) {
			Floor* floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->Object::SetType(3);
			floor->SetNumber(objData.number);
			floor->SetInstanceNo(conveyorNum);
			conveyorNum++;
			return true;
		}
		//���O��spike�Ȃ�
		else if (objData.ForwardMatchName(L"spike") == true) {
			Spike* spike = NewGO<Spike>(0, "spike");
			spike->Object::SetPosition(objData.position);
			spike->Object::SetRotation(objData.rotation);
			spike->Object::SetType(objData.number);
			return true;
		}
		//���O��box�Ȃ�
		else if (objData.ForwardMatchName(L"box") == true) {
			Box* box = NewGO<Box>(0, "box");
			box->Object::SetPosition(objData.position);
			box->Object::SetType(objData.number);
			return true;
		}
		//���O��checkPoint�Ȃ�
		else if (objData.ForwardMatchName(L"checkPoint") == true) {
			CheckPoint* checkPoint = NewGO<CheckPoint>(0, "checkPoint");
			checkPoint->Object::SetPosition(objData.position);
			checkPoint->SetType(objData.number);
			return true;
		}
		//���O��ykan�Ȃ�
		else if (objData.EqualObjectName(L"ykan") == true) {
			GameManager* gameManager = FindGO<GameManager>("gameManager");
			if (!gameManager->GetGetYkan()) {
				Ykan* ykan = NewGO<Ykan>(0, "ykan");
				ykan->SetPosition(objData.position);
			}
			return true;
		}
		//���O��wall�Ȃ�
		else if (objData.ForwardMatchName(L"wall") == true) {
			Wall* wall = NewGO<Wall>(0, "wall");
			wall->SetPosition(objData.position);
			wall->SetNumber(objData.number);
			wall->SetRotation(objData.rotation);
			return true;
		}
		//���O��outside�Ȃ�
		else if (objData.ForwardMatchName(L"outside") == true) {
			Outside* outside = NewGO<Outside>(0, "outside");
			outside->SetPosition(objData.position);
			outside->SetNumber(objData.number);
			return true;
		}
		//���O��invicibleWall�Ȃ�
		else if (objData.ForwardMatchName(L"invicibleWall") == true) {
			InvicibleWall* invicibleWall = NewGO<InvicibleWall>(0, "invicibleWall");
			invicibleWall->Object::SetPosition(objData.position);
			invicibleWall->SetRotation(objData.rotation);
			return true;
		}


		//���O��background�Ȃ�
		if (objData.EqualObjectName(L"background") == true) {

			return false;
		}
		return true;
		}
	);

	if (moveFloorNum != 0) {
		//�C���X�^���V���O�`��̐ݒ�
		FloorRender* moveFloorRender = NewGO<FloorRender>(0, "floorRender");
		moveFloorRender->SetMaxSphere(moveFloorNum);
		moveFloorRender->SetType(0);
	}

	if (fallFloorNum != 0) {
		//�C���X�^���V���O�`��̐ݒ�
		FloorRender* fallFloorRender = NewGO<FloorRender>(0, "floorRender");
		fallFloorRender->SetMaxSphere(fallFloorNum);
		fallFloorRender->SetType(0);
	}

	if (conveyorNum != 0) {
		//�C���X�^���V���O�`��̐ݒ�
		FloorRender* conveyorRender = NewGO<FloorRender>(0, "floorRender");
		conveyorRender->SetMaxSphere(conveyorNum);
		conveyorRender->SetType(3);
	}

	//�ݒ肵�����W�ɏ����鏰����������
	vanishFloorManager->SpawnFloor();

	//�X�J�C�L���[�u�̈ʒu��ݒ�
	//Player* player = FindGO<Player>("player");
	//m_skyCube->SetPosition(Vector3(player->GetPosition().x, 0.0f, 0.0f));

	delete m_levelRender;
}

void Stage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	//���x���œǂݍ��񂾃��f����\��������B
	m_levelRender->Draw(rc);
}