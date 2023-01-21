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

	//座標を取得
	inline Vector3 GetPosition() { return m_position; }
	//ボスの種類を設定
	inline void SetNumber(const int& bossNum) { m_bossNum = bossNum; }
	//攻撃力を取得
	inline int GetPower() { return m_damage; }

private:
	//移動処理
	void Move();
	//攻撃処理
	void Attack();
	//当たり判定の処理
	void Collision();
	//共通のステート遷移処理
	void ProcessCommonStateTransition();
	//移動時のステート遷移処理
	void ProcessMoveStateTransition();
	//攻撃時のステート遷移処理
	void ProcessAttackStateTransition();
	//特殊攻撃時のステート遷移処理
	void ProcessSpecialAttackStateTransition();
	//死亡時のステート遷移処理
	void ProcessDeadStateTransition();
	//ステート管理
	void ManageState();
	//アニメーション処理
	void PlayAnimation();
	//ダメージ処理
	void ReceiveDamage(const int damage) override;
	//攻撃可能かどうか
	bool CanAttack();
	//ジャンプ可能かどうか
	bool CanJump();

	/// <summary>
	/// ステータスを設定
	/// </summary>
	/// <param name="receiveDamage">受ダメージ</param>
	/// <param name="damage">被ダメージ</param>
	/// <param name="atkPer">攻撃する割合</param>
	/// <param name="speed">移動速度</param>
	/// <param name="jumpSpeed">ジャンプ速度</param>
	/// <param name="colScale">当たり判定の大きさ</param>
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


	bool m_isJump = false;			//ジャンプ中なら
	bool m_action = false;

	int m_selectWeapon = 0;			//選択した武器
	int m_receiveDamage = 0;		//受ダメージ

	float m_gravity = 0.0f;			//重力
	float m_animationTime = 0.0f;	//アニメーションの時間
	float m_attackTime = 0.0f;		//攻撃する時間

	int m_bossNum = 0;				//ボスの種類
	int m_damage = 0;				//与ダメージ
	int m_atkPercent = 0;			//攻撃の割合
	float m_speed = 0.0f;			//速度
	float m_jumpSpeed = 0.0f;		//ジャンプ速度
	float m_atkT = 0.0f;			//攻撃可能時間
	float m_animT = 0.0f;			//アニメーション時間
	float m_clearTime = 0.0f;

};

