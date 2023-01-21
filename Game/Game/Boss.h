#pragma once
#include "Actor.h"


class Player;
class Barrett;
class Weapon;

class Boss : public IGameObject, public Actor
{
public:
	enum EnBossState {
		enBossState_Idle,
		enBossState_Move,
		enBossState_Jump,
		enBossState_Attack,
		enBossState_SpecialAttack,
		enBossState_Dead,
	};

public:
	Boss();
	~Boss();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

	//���W���擾
	inline Vector3 GetPosition() { return m_position; }
	//�{�X�̎�ނ�ݒ�
	inline void SetNumber(const int& bossNum) { m_bossNum = bossNum; }
	//�U���͂��擾
	inline int GetPower() { return m_damage; }

private:
	//�ړ�����
	void Move();
	//�U������
	void Attack();
	//�����蔻��̏���
	void Collision();
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�ړ����̃X�e�[�g�J�ڏ���
	void ProcessMoveStateTransition();
	//�U�����̃X�e�[�g�J�ڏ���
	void ProcessAttackStateTransition();
	//����U�����̃X�e�[�g�J�ڏ���
	void ProcessSpecialAttackStateTransition();
	//���S���̃X�e�[�g�J�ڏ���
	void ProcessDeadStateTransition();
	//�X�e�[�g�Ǘ�
	void ManageState();
	//�A�j���[�V��������
	void PlayAnimation();
	//�_���[�W����
	void ReceiveDamage(const int damage) override;
	//�U���\���ǂ���
	bool CanAttack();
	//�W�����v�\���ǂ���
	bool CanJump();

	/// <summary>
	/// �X�e�[�^�X��ݒ�
	/// </summary>
	/// <param name="receiveDamage">��_���[�W</param>
	/// <param name="damage">��_���[�W</param>
	/// <param name="atkPer">�U�����銄��</param>
	/// <param name="speed">�ړ����x</param>
	/// <param name="jumpSpeed">�W�����v���x</param>
	/// <param name="colScale">�����蔻��̑傫��</param>
	void SetStatus(int receiveDamage, int damage, int atkper, float speed, float jumpSpeed, float animT, Vector3 colScale)
	{
		m_receiveDamage = receiveDamage;
		m_damage = damage;
		m_atkPercent = atkper;
		m_speed = speed;
		m_jumpSpeed = jumpSpeed;
		m_animT = animT;
		m_colScale = colScale;
	}

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Attack,
		enAnimationClip_Num
	};
	AnimationClip			m_animationClip[enAnimationClip_Num];
	SpriteRender			m_hpSpriteRender[15];
	SpriteRender			m_hpBarSpriteRender;

	FontRender m_debugFontRender;

	CollisionObject*		m_collisionObject;

	Player*					player = nullptr;
	Barrett*				barrett[4] = {nullptr, nullptr, nullptr, nullptr};
	Weapon*					weapon = nullptr;

	Vector3					m_firstPos;
	Vector3					m_colScale;
	Quaternion				m_rotation;

	EnBossState				m_bossState = enBossState_Idle;


	bool m_isJump = false;			//�W�����v���Ȃ�
	bool m_action = false;

	int m_selectWeapon = 0;			//�I����������
	int m_receiveDamage = 0;		//��_���[�W

	float m_gravity = 0.0f;			//�d��
	float m_animationTime = 0.0f;	//�A�j���[�V�����̎���
	float m_attackTime = 0.0f;		//�U�����鎞��

	int m_bossNum = 0;				//�{�X�̎��
	int m_damage = 0;				//�^�_���[�W
	int m_atkPercent = 0;			//�U���̊���
	float m_speed = 0.0f;			//���x
	float m_jumpSpeed = 0.0f;		//�W�����v���x
	float m_atkT = 0.0f;			//�U���\����
	float m_animT = 0.0f;			//�A�j���[�V��������
	float m_clearTime = 0.0f;

};

