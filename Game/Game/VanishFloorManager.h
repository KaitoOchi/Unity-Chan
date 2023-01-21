#pragma once


class Floor;

class VanishFloorManager : public IGameObject
{
public:
	VanishFloorManager();
	~VanishFloorManager();
	bool Start();
	void Update();

	//���W�̐ݒ�
	inline void SetPosition(const Vector3& pos, const int num) { m_floorPosition[num] = pos; }

	/// <summary>
	/// ���ʂ�ݒ肷�鏈��
	/// </summary>
	/// <param name="vol">����</param>
	void SetSoundVolume(float vol) { m_volume = vol; }
	
	//�o�^�������W����ɑ���𐶐�����
	void SpawnFloor();

private:
	Floor*		floor[10];

	Vector3		m_floorPosition[10];
	int			m_frameCount = 0;		//�t���[���J�E���g
	int			m_floorNum = 0;			//���ԍ�
	int			m_activeNum = 0;		//�\����Ԃ̏��ԍ�
	float		m_volume = 0.0f;		//����
};

