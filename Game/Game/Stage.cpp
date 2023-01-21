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

	//ステージを作成
	m_modelRender.Init(m_stageName[m_stageNum]);
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_backGroundModelRender.SetRaytracingWorld(false);
	m_backGroundModelRender.Init(m_backGroundNum[m_stageNum]);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetLuminance(10.0f);

	//ディレクショナルライト
	g_renderingEngine->SetDirectionLight(
		0,					// ライトの番号
		{ 1, -1, 0.75 },			// 方向
		{ 200, 200, 200 }		// カラー
	);
	//アンビエントライト
	g_renderingEngine->SetAmbient({ 2, 2, 2 });
	//レイトレーシングを有効
	//g_renderingEngine->EnableRaytracing();
	//トーンマップ、デフォルトは0.18
	g_renderingEngine->SetSceneMiddleGray(0.48f);
	//ブルーム
	g_renderingEngine->SetBloomThreshold(10.0f);
	//カスケードシャドウの近影エリア率
	g_renderingEngine->SetCascadeNearAreaRates(0.05f, 0.5f, 0.9f);

	// 環境光の計算のためのIBLテクスチャをセットする。
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

	//ステージ遷移中なら
	if (changeStage) {
		m_courceNum++;
		m_levelRender = new LevelRender;
		InitLevelRender();
		return;
	}

	//クリアしたかどうか
	if (isClear) {
		NewGO<GameClear>(3, "gameClear");
		DeleteGO(this);
	}
	//ゲームオーバー状態かどうか
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

	//レベルデザイン処理
	m_levelRender->Init(m_courceName[m_courceNum], [&](LevelObjectData& objData) {

		//名前がenemyなら
		if (objData.ForwardMatchName(L"enemy") == true) {
			Enemy* enemy = NewGO<Enemy>(0, "enemy");
			enemy->Actor::SetPosition(objData.position);
			enemy->SetNumber(objData.number);
			enemy->Actor::SetSoundVolume(gameManager->m_sfx);
			return true;
		}
		//名前がbossなら
		else if (objData.ForwardMatchName(L"boss") == true) {
			Boss* boss = NewGO<Boss>(0, "boss");
			boss->Actor::SetPosition(objData.position);
			boss->SetNumber(objData.number);
			boss->Actor::SetSoundVolume(gameManager->m_sfx);
		}
		//名前がladderなら
		else if (objData.EqualObjectName(L"ladder") == true) {
			Ladder* ladder = NewGO<Ladder>(0, "ladder");
			ladder->Object::SetPosition(objData.position);
			ladder->SetHeight(300.0f);
			return true;
		}
		//名前がspringなら
		else if (objData.EqualObjectName(L"spring") == true) {
			Spring* spring = NewGO<Spring>(0, "spring");
			spring->Object::SetPosition(objData.position);
			return true;
		}
		//名前がmoveFloorなら
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
		//名前がmoveFloorなら
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
		//名前がfallFloorなら
		else if (objData.EqualObjectName(L"fallFloor") == true) {
			Floor* floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->Object::SetType(1);
			floor->SetInstanceNo(fallFloorNum);
			fallFloorNum++;
			return true;
		}
		//名前がvanishFloorなら
		else if (objData.ForwardMatchName(L"vanishFloor") == true) {
			vanishFloorManager->SetPosition(objData.position, objData.number);
			return true;
		}
		//名前がconveyorなら
		else if (objData.ForwardMatchName(L"conveyor") == true) {
			Floor* floor = NewGO<Floor>(0, "floor");
			floor->Object::SetPosition(objData.position);
			floor->Object::SetType(3);
			floor->SetNumber(objData.number);
			floor->SetInstanceNo(conveyorNum);
			conveyorNum++;
			return true;
		}
		//名前がspikeなら
		else if (objData.ForwardMatchName(L"spike") == true) {
			Spike* spike = NewGO<Spike>(0, "spike");
			spike->Object::SetPosition(objData.position);
			spike->Object::SetRotation(objData.rotation);
			spike->Object::SetType(objData.number);
			return true;
		}
		//名前がboxなら
		else if (objData.ForwardMatchName(L"box") == true) {
			Box* box = NewGO<Box>(0, "box");
			box->Object::SetPosition(objData.position);
			box->Object::SetType(objData.number);
			return true;
		}
		//名前がcheckPointなら
		else if (objData.ForwardMatchName(L"checkPoint") == true) {
			CheckPoint* checkPoint = NewGO<CheckPoint>(0, "checkPoint");
			checkPoint->Object::SetPosition(objData.position);
			checkPoint->SetType(objData.number);
			return true;
		}
		//名前がykanなら
		else if (objData.EqualObjectName(L"ykan") == true) {
			GameManager* gameManager = FindGO<GameManager>("gameManager");
			if (!gameManager->GetGetYkan()) {
				Ykan* ykan = NewGO<Ykan>(0, "ykan");
				ykan->SetPosition(objData.position);
			}
			return true;
		}
		//名前がwallなら
		else if (objData.ForwardMatchName(L"wall") == true) {
			Wall* wall = NewGO<Wall>(0, "wall");
			wall->SetPosition(objData.position);
			wall->SetNumber(objData.number);
			wall->SetRotation(objData.rotation);
			return true;
		}
		//名前がoutsideなら
		else if (objData.ForwardMatchName(L"outside") == true) {
			Outside* outside = NewGO<Outside>(0, "outside");
			outside->SetPosition(objData.position);
			outside->SetNumber(objData.number);
			return true;
		}
		//名前がinvicibleWallなら
		else if (objData.ForwardMatchName(L"invicibleWall") == true) {
			InvicibleWall* invicibleWall = NewGO<InvicibleWall>(0, "invicibleWall");
			invicibleWall->Object::SetPosition(objData.position);
			invicibleWall->SetRotation(objData.rotation);
			return true;
		}


		//名前がbackgroundなら
		if (objData.EqualObjectName(L"background") == true) {

			return false;
		}
		return true;
		}
	);

	if (moveFloorNum != 0) {
		//インスタンシング描画の設定
		FloorRender* moveFloorRender = NewGO<FloorRender>(0, "floorRender");
		moveFloorRender->SetMaxSphere(moveFloorNum);
		moveFloorRender->SetType(0);
	}

	if (fallFloorNum != 0) {
		//インスタンシング描画の設定
		FloorRender* fallFloorRender = NewGO<FloorRender>(0, "floorRender");
		fallFloorRender->SetMaxSphere(fallFloorNum);
		fallFloorRender->SetType(0);
	}

	if (conveyorNum != 0) {
		//インスタンシング描画の設定
		FloorRender* conveyorRender = NewGO<FloorRender>(0, "floorRender");
		conveyorRender->SetMaxSphere(conveyorNum);
		conveyorRender->SetType(3);
	}

	//設定した座標に消える床を召喚する
	vanishFloorManager->SpawnFloor();

	//スカイキューブの位置を設定
	//Player* player = FindGO<Player>("player");
	//m_skyCube->SetPosition(Vector3(player->GetPosition().x, 0.0f, 0.0f));

	delete m_levelRender;
}

void Stage::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

	//レベルで読み込んだモデルを表示させる。
	m_levelRender->Draw(rc);
}