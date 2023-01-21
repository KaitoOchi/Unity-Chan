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

	//座標を設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//方向を設定
	inline void SetDirection(const int dir) {m_playerDirection = dir;}
	//使用者を設定
	inline void SetUser(const EnUser& user) { m_enUser = user; }
	//攻撃力を設定
	inline void SetPower(const int pow) { m_power = pow; }
	//速度を設定
	inline void SetSpeed(const float spd) { m_speed = spd; }
	//追尾するかを設定
	inline void SetChase(const bool chase) { m_canChase = chase; }

	/// <summary>
	/// 音量を設定する処理
	/// </summary>
	/// <param name="vol">音量</param>
	void SetSoundVolume(float vol) { m_volume = vol; }

private:
	//当たり判定の処理
	void Collision();
	//リセット処理
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

	bool m_shot = false;			//発射時にtrue
	bool m_canChase = false;		//追尾するかどうか
	bool m_isRepel = false;			//弾かれているかどうか

	int m_playerDirection = 0;		//プレイヤーの方向
	int m_power = 0;				//攻撃力

	float m_speed = 0.0f;			//速度
	float m_volume = 0.0f;			//音量
};

