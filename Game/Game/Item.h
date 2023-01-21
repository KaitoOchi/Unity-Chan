#pragma once


class Player;

class Item : public IGameObject
{
public:
	Item();
	~Item();
	bool Start();
	void Update();

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }

private:
	EffectEmitter*		m_itemEffect = nullptr;
	Player*				player = nullptr;
	Vector3				m_position;

	int					m_heal = 0;					//�񕜗�
	int					m_difference = 0;			//�񕜂�����p�����[�^
	float				m_volume = 0.0f;
};