#pragma once

class Player;
class Enemy;
class Boss;

class Weapon : public IGameObject
{
public:
	enum enUser {
		enUser_None,
		enUser_Player,
		enUser_Enemy,
		enUser_Boss
	};

public:
	Weapon();
	~Weapon();
	bool Start();
	void Update();

	//���W��ݒ�
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//������ݒ�
	inline void SetDirection(const int dir) { m_playerDirection = dir; }
	//�e�̎�ނ�ݒ�
	inline void SetWeaponNum(const int i) { m_weaponNum = i; }
	//�g�p�҂�ݒ�
	inline void SetUser(const enUser& user) { m_enUser = user; }
	//�U���͂�ݒ�
	inline void SetPower(const int pow) { m_power = pow; }

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
	Boss*					boss = nullptr;

	Vector3					m_position;
	Vector3					m_userPos;
	Vector3					m_oldPos;
	Quaternion				m_rotation;

	enUser					m_enUser = enUser_None;

	bool m_shot = false;			//���ˎ���true

	int m_playerDirection = 0;		//�v���C���[�̕���
	int m_effectRotation = 0;		//�G�t�F�N�g�̊p�x
	int m_weaponNum = 0;			//����̎��
	int m_power = 0;				//�U����

	float m_volume = 0.0f;			//����
};