#pragma once

class Player;
class Enemy;
class Stage;
class Box;

class Barrett : public IGameObject
{
public:
	enum EnUser {
		enUser_None,
		enUser_Player,
		enUser_Enemy,
		enUser_Boss
	};

public:
	Barrett();
	~Barrett();
	bool Start();
	void Update();

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//������ݒ�
	inline void SetDirection(const int dir) {m_playerDirection = dir;}
	//�g�p�҂�ݒ�
	inline void SetUser(const EnUser& user) { m_enUser = user; }
	//�U���͂�ݒ�
	inline void SetPower(const int pow) { m_power = pow; }
	//���x��ݒ�
	inline void SetSpeed(const float spd) { m_speed = spd; }
	//�ǔ����邩��ݒ�
	inline void SetChase(const bool chase) { m_canChase = chase; }

	/// <summary>
	/// ���ʂ�ݒ肷�鏈��
	/// </summary>
	/// <param name="vol">����</param>
	void SetSoundVolume(float vol) { m_volume = vol; }

private:
	//�����蔻��̏���
	void Collision();
	//���Z�b�g����
	void Reset();

	CollisionObject*		m_collisionObject = nullptr;
	EffectEmitter*			m_barrettEffect = nullptr;

	Player*					player = nullptr;
	Enemy*					enemy = nullptr;
	Stage*					stage = nullptr;
	Box*					box = nullptr;

	Vector3					m_position;
	Vector3					m_diff;

	EnUser					m_enUser = enUser_None;

	bool m_shot = false;			//���ˎ���true
	bool m_canChase = false;		//�ǔ����邩�ǂ���
	bool m_isRepel = false;			//�e����Ă��邩�ǂ���

	int m_playerDirection = 0;		//�v���C���[�̕���
	int m_power = 0;				//�U����

	float m_speed = 0.0f;			//���x
	float m_volume = 0.0f;			//����
};

