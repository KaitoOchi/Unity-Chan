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

	//座標を設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//方向を設定
	inline void SetDirection(const int dir) { m_playerDirection = dir; }
	//弾の種類を設定
	inline void SetWeaponNum(const int i) { m_weaponNum = i; }
	//使用者を設定
	inline void SetUser(const enUser& user) { m_enUser = user; }
	//攻撃力を設定
	inline void SetPower(const int pow) { m_power = pow; }

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
	Boss*					boss = nullptr;

	Vector3					m_position;
	Vector3					m_userPos;
	Vector3					m_oldPos;
	Quaternion				m_rotation;

	enUser					m_enUser = enUser_None;

	bool m_shot = false;			//発射時にtrue

	int m_playerDirection = 0;		//プレイヤーの方向
	int m_effectRotation = 0;		//エフェクトの角度
	int m_weaponNum = 0;			//武器の種類
	int m_power = 0;				//攻撃力

	float m_volume = 0.0f;			//音量
};