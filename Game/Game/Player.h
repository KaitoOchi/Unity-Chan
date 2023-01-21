#pragma once
#include "Actor.h"

class Game;
class GameManager;
class Barrett;
class Weapon;

class Player : public IGameObject, public Actor
{
public:
	enum EnPlayerState {
		enPlayerState_Idle,
		enPlayerState_Move,
		enPlayerState_Jump,
		enPlayerState_Attack,
		enPlayerState_Climb_Idle,
		enPlayerState_Climb,
		enPlayerState_Sliding,
		enPlayerState_WallJump,
		enPlayerState_Damage,
		enPlayerState_IsBigin,
		enPlayerState_Scroll,
		enPlayerState_Dead,
		enPlayerState_Clear
	};

public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

	/// <summary>
	/// �_���[�W���󂯂鏈��
	/// </summary>
	/// <param name="damage">��_���[�W</param>
	/// <param name="ivTime">���G����</param>
	void ReceiveDamage(int damage) override;

	/// <summary>
	/// �񕜂��鏈��
	/// </summary>
	/// <param name="heal">�񕜗�</param>
	/// <param name="differencel">�񕜂�����p�����[�^�B0��HP�A1��MP</param>
	void Heal(int heal, bool difference);

	//�����̏���
	void Font();

	//�f�o�b�O�p
	void Debug();

	//�v���C���[�̍��W���擾
	inline Vector3 GetPosition() { return m_position; }
	//�v���C���[�̍U���͂��擾
	inline int GetWeaponPower() { return m_weaponPower[m_selectWeapon]; }
	//�v���C���[��HP���擾
	inline int GetHP() { return m_hp; }
	//�L�����R�����擾
	inline CharacterController& GetCharacterController() { return m_characterController; }
	//�����I���ł��邩�ݒ�
	inline void SetCanSelectWeapon(int canSelectWeapon) { m_canSelectWeapon = canSelectWeapon; }
	//�ړ����x�����Z���鏈��
	inline void AddMoveSpeed(const Vector3& addMoveSpeed) { m_addMoveSpeed += addMoveSpeed; }
	//�n�V�S����̏���
	inline void SetCanClimb(const bool canClimb) { m_canClimb = canClimb; }
	//�o�l����̏���
	inline void SetIsSpring(const bool isSpring) { m_isSpring = isSpring; }

	//�X�N���[���X�e�[�g�ɑJ��
	inline void SetScrollState() { m_enPlayerState = enPlayerState_Scroll; }
	//�X�N���[���J�ڒ����ǂ���
	inline const bool GetScrollState() { return m_enPlayerState == enPlayerState_Scroll; }

private:
	//�ړ�����
	void MoveX();
	//�W�����v����
	void MoveY();
	//��]����
	void Rotation();
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�U�����̃X�e�[�g�J�ڏ���
	void ProcessAttackStateTransition();
	//�X���C�f�B���O���̃X�e�[�g�J�ڏ���
	void ProcessSlidingStateTransition();
	//�ǃW�����v���̃X�e�[�g�J�ڏ���
	void ProcessWallJumpStateTransition();
	//�_���[�W���̃X�e�[�g�J�ڏ���
	void ProcessDamageStateTransition();
	//�J�n���̃X�e�[�g�J�ڏ���
	void ProcessIsBiginStateTransition();
	//�X�N���[�����̃X�e�[�g�J�ڏ���
	void ProcessScrollStateTransition();
	//���S���̃X�e�[�g�J�ڏ���
	void ProcessDeadStateTransition();
	//�N���A���̃X�e�[�g�J�ڏ���
	void ProcessClearStateTransition();
	//�X�e�[�g�Ǘ�
	void ManageState();
	//�A�j���[�V��������
	void PlayAnimation();
	//�e�̑I������
	void SelectWeapon();
	//�e�̔��ˏ���
	void Shot();
	//���S����
	void Dead();

	//�������Ԃ��ǂ���
	inline bool IsEnableMove() const
	{
		return m_enPlayerState != enPlayerState_Attack &&
			m_enPlayerState != enPlayerState_Climb_Idle &&
			m_enPlayerState != enPlayerState_Climb &&
			m_enPlayerState != enPlayerState_Damage;
	}

	//�e�����Ă��Ԃ��ǂ���
	inline bool IsEnableShot() const
	{
		return m_enPlayerState != enPlayerState_Sliding &&
			m_enPlayerState != enPlayerState_Damage;
	}

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_Sliding,
		enAnimationClip_Climb_Idle,
		enAnimationClip_Climb,
		enAnimationClip_Attack,
		enAnimationClip_Damage,
		enAnimationClip_Clear,
		enAnimationClip_IsBigin,
		enAnimationClip_Num,
	};
	AnimationClip			animationClips[enAnimationClip_Num];

	CharacterController		m_characterController;
	SpriteRender			m_playerHPSpriteRender[15];
	SpriteRender			m_playerMPSpriteRender[15];
	SpriteRender			m_playerMPBarSpriteRender;
	SpriteRender			m_playerWeaponSpriteRender[5];
	FontRender				m_fontRender;
	FontRender				m_debugFontRender;

	Game*					game = nullptr;
	GameManager*			gameManager = nullptr;
	Barrett*				barrett[4] = { nullptr, nullptr, nullptr, nullptr };
	Weapon*					weapon = nullptr;

	Vector3					m_stickL;
	Vector3					m_addMoveSpeed;
	Vector3					m_oldPos;

	EnPlayerState			m_enPlayerState = enPlayerState_IsBigin;

	bool m_isSliding = false;		//�X���C�f�B���O�����Ă��邩�ǂ���
	bool m_isClimb = false;			//�o���Ă��邩�ǂ���
	bool m_isSpring = false;		//�o�l���g�p�������ǂ���
	bool m_canSelectWeapon = true;	//�����I���ł��邩�ǂ���
	bool m_canClimb = false;		//�n�V�S���g���邩�ǂ���
	bool m_healDifference = false;	//�񕜂�����
	bool m_isMove = false;			//�ړ����Ă��邩�ǂ���

	int m_jumpCount = 0;			//�W�����v��
	int m_selectWeapon = 0;			//�I����������
	int m_maxJumpTime = 2;			//�ő�W�����v��
	int m_frameCount = 0;			//�t���[���J�E���g
	int m_heal = 0;					//�񕜗�
	int m_oldDirection = 1;

	float m_accel = 0.0f;			//����
	float m_gravityAccel = 0.0f;	//�d�͉���
	float m_jumpTime = 0.0f;		//�W�����v���̎���
	float m_scrollTime = 0.0f;		//�X�N���[������
	float m_wallJumpTime = 0.0f;

	bool m_canUseWeapon[5] = { true, true, true, true, false };		//���킪�g���邩�ǂ���
	int m_weaponPower[5] = { 1, 1, 3, 2, 12 };		//����̍U����
	int m_weaponMP[5] = { 0, 0, 0, 0, 0 };			//�����MP
};

