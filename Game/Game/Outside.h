#pragma once


class Player;

class Outside : public IGameObject {

public:
	Outside();
	~Outside();
	bool Start();
	void Update();

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//���W�̎擾
	inline Vector3 GetPosition() { return m_position; }
	//�ԍ���ݒ�
	inline void SetNumber(const int num) { m_number = num; }
	//�ԍ��̎擾
	inline int GetNumber() { return m_number; }

private:
	Player* player = nullptr;

	Vector3 m_position;

	int m_number = 0;
};