#pragma once
#include "Actor.h"


class Player;
class Barrett;
class Stage;

class Enemy : public IGameObject, public Actor
{
public:
	enum MoveType {
		enMove_X,
		enMove_Y,
		enMove_XY,
		enMove_Chase_Ground,
		enMove_Chase_Air,
		enMove_Stop
	};

	enum AttackType {
		enAttack_Barrett,
		enAttack_Weapon,
		enAttack_Summon,
		enAttack_Not
	};

public:
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

	/// <summary>
	/// �_���[�W���v�Z���鏈��
	/// </summary>
	/// <param name="damage">��_���[�W</param>
	void ReceiveDamage(int damage) override;

	//�G�̎�ނ�ݒ�
	inline void SetNumber(const int& num) { m_enemyNum = num; }
	//�G�̍U���͂��擾
	inline int GetPower() { return m_damage; }

private:
	//�ړ�����
	void Move();
	//�ݒu����
	void IsOnGround();
	//�U������
	void Attack();
	//�����蔻��̏���
	void Collision();
	//�X�e�[�g�Ǘ�
	void ManageState();
	//�A�j���[�V��������
	void PlayAnimation();
	//���Z�b�g����
	void Reset();
	//���S����
	void Dead();
	//������ύX
	void Direction();

	/// <summary>
	/// �X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="maxHP">�ő�HP</param>
	/// <param name="damage">��_���[�W</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="revive">�����������邩�ǂ���</param>
	/// <param name="interval">�U���Ԋu</param>
	/// <param name="moveN">�ړ��l</param>
	/// <param name="moveT">�ړ�����</param>
	/// <param name="colScale">�����蔻��̑傫��</param>
	/// <param name="mType">�s��������</param>
	/// <param name="aType">�U��������</param>
	void SetStatus(int maxHP, int damage, float speed, bool revive, int interval, int moveN, float moveT, Vector3 colScale, MoveType mType, AttackType aType)
	{
		m_maxHP = maxHP;
		m_damage = damage;
		m_speed = speed;
		m_isCanRevive = revive;
		m_attackInterval = interval;
		m_moveNum = moveN;
		m_moveT = moveT;
		m_colScale = colScale;
		m_moveType = mType;
		m_attackType = aType;
	}

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];


	CollisionObject*		m_collisionObject;
	Player*					player = nullptr;
	Barrett*				barrett = nullptr;
	Enemy*					enemy[3] = { nullptr, nullptr, nullptr };
	Stage*					stage = nullptr;
	Vector3					m_firstPos;
	Vector3					m_colScale;

	MoveType				m_moveType = enMove_Stop;
	AttackType				m_attackType = enAttack_Not;

	bool m_active = false;		//�A�N�e�B�u���ǂ���
	bool m_isOnGround = true;	//�n�ʂƐڒn���Ă��邩�ǂ���
	bool m_canSpawn = true;		//�X�|�[���\���ǂ���

	int m_enemyState = 0;		//�A�j���[�V�����̃X�e�[�g
	int m_moveDirection = 1;	//�ړ��������
	int m_enemyNum = 0;			//�G�̓����i���o�[

	float m_gravity = 0.0f;		//�d��
	float m_moveTime = 0.0f;	//�ړ����鎞��
	float m_addSpeed = 0.0f;	//�t�^�����X�s�[�h


	bool m_isCanRevive = false;		//�����������邩
	int m_maxHP = 0;				//�ő�HP
	int m_damage = 0;				//�^�_���[�W
	int m_moveNum = 0;				//�ړ���
	int m_attackInterval = 0;		//�U���Ԋu

	float m_moveT = 0.0f;		//���]���鎞��
	float m_speed = 0.0f;		//�ړ����x
};

