#pragma once

class Floor;
class FallFloorRender;
class GameManager;

class Stage : public IGameObject
{
public:
	Stage();
	~Stage();
	bool Start();
	void Render(RenderContext& rc);

	//�X�e�[�W�J�ڏ���
	void DeleteObject(bool changeStage, bool isClear, bool isGameOver);

	//�X�e�[�W�ԍ���ݒ�
	inline void SetStageNum(const int num) { m_stageNum = num; }
	//�R�[�X�ԍ���ݒ�
	inline void SetCourceNum(const int num) { m_courceNum = num; }
	//�����蔻����擾
	inline PhysicsStaticObject& GetPhysicsStaticObject() { return m_physicsStaticObject; }

private:
	//���x���f�U�C������
	void InitLevelRender();

	ModelRender				m_modelRender;
	ModelRender				m_backGroundModelRender;
	PhysicsStaticObject		m_physicsStaticObject;

	SkyCube*				m_skyCube = nullptr;
	LevelRender*			m_levelRender = nullptr;
	GameManager*			gameManager = nullptr;

	const char* m_stageName[4] = {  "Assets/modelData/stage/stage0.tkm",
									"Assets/modelData/stage/stage1.tkm",
									"Assets/modelData/stage/stage2.tkm",
									"Assets/modelData/stage/stage3.tkm",
	};

	const char* m_backGroundNum[4] = { "Assets/modelData/stage/background/stage1.tkm",
										"Assets/modelData/stage/background/stage1.tkm",
										"Assets/modelData/stage/background/stage1.tkm",
										"Assets/modelData/stage/background/stage1.tkm"
	};

	const char* m_courceName[9];


	const char* m_cource0_storage[9] = {"Assets/level3D/stage0_0_level.tkl",
										"Assets/level3D/stage0_1_level.tkl",
										"Assets/level3D/stage0_2_level.tkl",
										"Assets/level3D/stage0_3_level.tkl"
	};

	const char* m_cource1_storage[9] = {"Assets/level3D/stage1_0_level.tkl",
										"Assets/level3D/stage1_1_level.tkl",
										"Assets/level3D/stage1_2_level.tkl",
										"Assets/level3D/stage1_3_level.tkl",
										"Assets/level3D/stage1_4_level.tkl",
										"Assets/level3D/stage1_5_level.tkl",
										"Assets/level3D/stage1_6_level.tkl",
										"Assets/level3D/stage1_7_level.tkl",
										"Assets/level3D/stage1_8_level.tkl"
	};


	int m_stageNum = 0;					//�X�e�[�W�ԍ�
	int m_courceNum = 0;				//�R�[�X�ԍ�
};

