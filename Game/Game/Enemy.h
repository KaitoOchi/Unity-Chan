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
	/// ダメージを計算する処理
	/// </summary>
	/// <param name="damage">被ダメージ</param>
	void ReceiveDamage(int damage) override;

	//敵の種類を設定
	inline void SetNumber(const int& num) { m_enemyNum = num; }
	//敵の攻撃力を取得
	inline int GetPower() { return m_damage; }

private:
	//移動処理
	void Move();
	//設置処理
	void IsOnGround();
	//攻撃処理
	void Attack();
	//当たり判定の処理
	void Collision();
	//ステート管理
	void ManageState();
	//アニメーション処理
	void PlayAnimation();
	//リセット処理
	void Reset();
	//死亡処理
	void Dead();
	//方向を変更
	void Direction();

	/// <summary>
	/// ステータスを設定
	/// </summary>
	/// <param name="maxHP">最大HP</param>
	/// <param name="damage">被ダメージ</param>
	/// <param name="speed">移動速度</param>
	/// <param name="revive">即時復活するかどうか</param>
	/// <param name="interval">攻撃間隔</param>
	/// <param name="moveN">移動値</param>
	/// <param name="moveT">移動時間</param>
	/// <param name="colScale">当たり判定の大きさ</param>
	/// <param name="mType">行動する種類</param>
	/// <param name="aType">攻撃する種類</param>
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

	bool m_active = false;		//アクティブかどうか
	bool m_isOnGround = true;	//地面と接地しているかどうか
	bool m_canSpawn = true;		//スポーン可能かどうか

	int m_enemyState = 0;		//アニメーションのステート
	int m_moveDirection = 1;	//移動する方向
	int m_enemyNum = 0;			//敵の内部ナンバー

	float m_gravity = 0.0f;		//重力
	float m_moveTime = 0.0f;	//移動する時間
	float m_addSpeed = 0.0f;	//付与されるスピード


	bool m_isCanRevive = false;		//即時復活するか
	int m_maxHP = 0;				//最大HP
	int m_damage = 0;				//与ダメージ
	int m_moveNum = 0;				//移動量
	int m_attackInterval = 0;		//攻撃間隔

	float m_moveT = 0.0f;		//反転する時間
	float m_speed = 0.0f;		//移動速度
};

