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
	/// ダメージを受ける処理
	/// </summary>
	/// <param name="damage">被ダメージ</param>
	/// <param name="ivTime">無敵時間</param>
	void ReceiveDamage(int damage) override;

	/// <summary>
	/// 回復する処理
	/// </summary>
	/// <param name="heal">回復量</param>
	/// <param name="differencel">回復させるパラメータ。0でHP、1でMP</param>
	void Heal(int heal, bool difference);

	//文字の処理
	void Font();

	//デバッグ用
	void Debug();

	//プレイヤーの座標を取得
	inline Vector3 GetPosition() { return m_position; }
	//プレイヤーの攻撃力を取得
	inline int GetWeaponPower() { return m_weaponPower[m_selectWeapon]; }
	//プレイヤーのHPを取得
	inline int GetHP() { return m_hp; }
	//キャラコンを取得
	inline CharacterController& GetCharacterController() { return m_characterController; }
	//武器を選択できるか設定
	inline void SetCanSelectWeapon(int canSelectWeapon) { m_canSelectWeapon = canSelectWeapon; }
	//移動速度を加算する処理
	inline void AddMoveSpeed(const Vector3& addMoveSpeed) { m_addMoveSpeed += addMoveSpeed; }
	//ハシゴ判定の処理
	inline void SetCanClimb(const bool canClimb) { m_canClimb = canClimb; }
	//バネ判定の処理
	inline void SetIsSpring(const bool isSpring) { m_isSpring = isSpring; }

	//スクロールステートに遷移
	inline void SetScrollState() { m_enPlayerState = enPlayerState_Scroll; }
	//スクロール遷移中かどうか
	inline const bool GetScrollState() { return m_enPlayerState == enPlayerState_Scroll; }

private:
	//移動処理
	void MoveX();
	//ジャンプ処理
	void MoveY();
	//回転処理
	void Rotation();
	//共通のステート遷移処理
	void ProcessCommonStateTransition();
	//攻撃時のステート遷移処理
	void ProcessAttackStateTransition();
	//スライディング時のステート遷移処理
	void ProcessSlidingStateTransition();
	//壁ジャンプ時のステート遷移処理
	void ProcessWallJumpStateTransition();
	//ダメージ時のステート遷移処理
	void ProcessDamageStateTransition();
	//開始時のステート遷移処理
	void ProcessIsBiginStateTransition();
	//スクロール時のステート遷移処理
	void ProcessScrollStateTransition();
	//死亡時のステート遷移処理
	void ProcessDeadStateTransition();
	//クリア時のステート遷移処理
	void ProcessClearStateTransition();
	//ステート管理
	void ManageState();
	//アニメーション処理
	void PlayAnimation();
	//弾の選択処理
	void SelectWeapon();
	//弾の発射処理
	void Shot();
	//死亡処理
	void Dead();

	//動ける状態かどうか
	inline bool IsEnableMove() const
	{
		return m_enPlayerState != enPlayerState_Attack &&
			m_enPlayerState != enPlayerState_Climb_Idle &&
			m_enPlayerState != enPlayerState_Climb &&
			m_enPlayerState != enPlayerState_Damage;
	}

	//弾を撃てる状態かどうか
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

	bool m_isSliding = false;		//スライディングをしているかどうか
	bool m_isClimb = false;			//登っているかどうか
	bool m_isSpring = false;		//バネを使用したかどうか
	bool m_canSelectWeapon = true;	//武器を選択できるかどうか
	bool m_canClimb = false;		//ハシゴを使えるかどうか
	bool m_healDifference = false;	//回復する種類
	bool m_isMove = false;			//移動しているかどうか

	int m_jumpCount = 0;			//ジャンプ回数
	int m_selectWeapon = 0;			//選択した武器
	int m_maxJumpTime = 2;			//最大ジャンプ回数
	int m_frameCount = 0;			//フレームカウント
	int m_heal = 0;					//回復量
	int m_oldDirection = 1;

	float m_accel = 0.0f;			//加速
	float m_gravityAccel = 0.0f;	//重力加速
	float m_jumpTime = 0.0f;		//ジャンプ中の時間
	float m_scrollTime = 0.0f;		//スクロール時間
	float m_wallJumpTime = 0.0f;

	bool m_canUseWeapon[5] = { true, true, true, true, false };		//武器が使えるかどうか
	int m_weaponPower[5] = { 1, 1, 3, 2, 12 };		//武器の攻撃力
	int m_weaponMP[5] = { 0, 0, 0, 0, 0 };			//武器のMP
};

