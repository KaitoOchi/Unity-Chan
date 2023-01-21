#pragma once


class FloorRender : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���̎�ނ�ݒ肷��B
	/// </summary>
	/// <param name="num"></param>
	inline void SetType(const int type) { m_type = type; }

	/// <summary>
	/// ���̍ő吔��ݒ肷��B
	/// </summary>
	/// <param name="numSphere"></param>
	void SetMaxSphere(int max)
	{
		m_maxFloor = max;
	}

	/// <summary>
	/// �C���X�^���V���O�f�[�^�̍X�V�B
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateInstancingData(
		int instanceNo,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

	/// <summary>
	/// �C���X�^���X��j���B
	/// </summary>
	/// <param name="instanceNo">�폜����C���X�^���X�̔ԍ�</param>
	void RemoveInstance(int instanceNo)
	{
		m_modelRender.RemoveInstance(instanceNo);
	}

private:
	ModelRender m_modelRender;

	const char* m_floorName[4] = { "Assets/modelData/object/moveFloor.tkm",
									"Assets/modelData/object/vanishFloor.tkm",
									"Assets/modelData/object/moveFloor.tkm",
									"Assets/modelData/object/conveyor.tkm"
								};

	int m_maxFloor = 0;		//���̍ő吔
	int m_type = 0;			//���̎��
};