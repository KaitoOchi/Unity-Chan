#pragma once

//アクターの基底クラス
class Actor
{
public:
	Actor();
	virtual ~Actor();
	virtual bool Start() = 0;
	virtual void Update() = 0;

	//座標を設定
	inline void SetPosition(const Vector3& pos) { m_position = pos; }
	//方向を取得
	inline int GetDirection() { return m_direction; }

	/// <summary>
	/// ダメージを計算する処理
	/// </summary>
	/// <param name="damage">被ダメージ</param>
	virtual void ReceiveDamage(int damage) = 0;

	/// <summary>
	/// 音量を設定する処理
	/// </summary>
	/// <param name="vol">音量</param>
	void SetSoundVolume(float vol) { m_volume = vol; }

protected:
	//点滅処理
	void Blink();

	ModelRender				m_modelRender;

	EffectEmitter*			m_effect = nullptr;

	Vector3					m_position;
	Vector3					m_moveSpeed;
	Quaternion				m_rotation;

	bool					m_enabled = true;		//表示状態
	int						m_hp = 0;				//HP
	int						m_direction = 1;		//方向
	float					m_stopTime = 0.0f;		//硬直時間
	float					m_ivTime = 0.0f;		//無敵時間
	float					m_volume = 0.0f;		//音量
};