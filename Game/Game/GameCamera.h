#pragma once

class Player;

class GameCamera : public IGameObject
{
public:
	enum EnState {
		enState_moveX,
		enState_moveY,
		enState_Idle,
		enState_Scroll
	};

public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	//x軸のみの移動
	inline void EnStateMoveX() { state = enState_moveX; }
	//y軸のみの移動
	inline void EnStateMoveY() { state = enState_moveY; }
	//移動させない
	inline void EnStateIdle() { state = enState_Idle; }
	//スクロールステート
	inline void EnStateScroll() { state = enState_Scroll; }
	//動けるかどうか
	inline bool IsIdle() const { return state != enState_Idle; }
	//カメラ座標の固定
	inline void SetFixedPosition(const Vector3& pos) { m_fixedPos = pos; }

	//カメラの移動方向を設定
	inline void SetDirection(const int dir)
	{
		if (dir == 0) {
			m_direction.x = 0;
			m_direction.y = 1;
		}
		else if (dir == 1) {
			m_direction.x = 1;
			m_direction.y = 0;
		}
		else if (dir == 2) {
			m_direction.x = 0;
			m_direction.y = -1;
		}
		else if (dir == 3) {
			m_direction.x = -1;
			m_direction.y = 0;
		}
	}

private:
	void SetCamera(const Vector3& target);

	Player* m_player = nullptr;

	Vector3 m_toCameraPos;
	Vector3 m_fixedPos;
	Vector2 m_direction;

	EnState state = enState_moveX;
};
